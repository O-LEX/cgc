#include "DER.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
#include <algorithm>

using namespace Eigen;

// calculate the parallel transport matrix from r1 to r2
Matrix3d ParallelTransport(const Vector3d& r1, const Vector3d& r2) {
    // ベクトルの正規化
    Vector3d r1_normalized = r1.normalized();
    Vector3d r2_normalized = r2.normalized();
    
    // 回転軸を計算（外積）
    Vector3d rotation_axis = r1_normalized.cross(r2_normalized);
    
    // 外積が0に近い場合（ベクトルが平行または反平行）の処理
    if (rotation_axis.norm() < 1e-10) {
        if (r1_normalized.dot(r2_normalized) > 0) {
            return Matrix3d::Identity();  // ベクトルが同じ方向
        } else {
            // ベクトルが反対方向の場合、180度回転
            return -Matrix3d::Identity();
        }
    }
    
    rotation_axis.normalize();
    
    // 回転角を計算
    double angle = acos(std::clamp(r1_normalized.dot(r2_normalized), -1.0, 1.0));
    
    // Rodriguesの回転公式を使用して回転行列を構築
    AngleAxisd rotation(angle, rotation_axis);
    return rotation.matrix();
}

// Vertex のコンストラクタ
Vertex::Vertex(const Eigen::Vector3d& pos) : position(pos), velocity(Eigen::Vector3d::Zero()) {}

// 等長セグメントにストランドを再分割する関数
void Strand::SubdivideStrand(int subdivisions) {
    if (vertices.size() < 2) return;

    // 総長さを計算
    double totalLength = 0.0;
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        totalLength += (vertices[i + 1].position - vertices[i].position).norm();
    }

    // 目標セグメント長を決定
    double targetLength = totalLength / subdivisions;
    std::vector<Vertex> newVertices;
    newVertices.emplace_back(vertices.front());

    double accumulated = 0.0;
    size_t current = 0;

    while (newVertices.size() < static_cast<size_t>(subdivisions) && current < vertices.size() - 1) {
        double segmentLength = (vertices[current + 1].position - vertices[current].position).norm();
        if (accumulated + segmentLength >= targetLength) {
            double remaining = targetLength - accumulated;
            double t = remaining / segmentLength;
            Eigen::Vector3d newPos = (1.0 - t) * vertices[current].position + t * vertices[current + 1].position;
            newVertices.emplace_back(newPos);
            accumulated = 0.0;
        } else {
            accumulated += segmentLength;
            ++current;
        }
    }

    // 最後の頂点を確実に追加
    newVertices.emplace_back(vertices.back().position);
    vertices = std::move(newVertices);
}

// フレームを計算する関数
void Strand::CalculateFrame() {
    if (vertices.size() < 2) return;

    tangent.clear();
    normal.clear();
    binormal.clear();

    // 初期フレームの設定
    Eigen::Vector3d t0 = (vertices[1].position - vertices[0].position).normalized();
    Eigen::Vector3d n0 = Eigen::Vector3d(-t0.y(), t0.x(), 0.0).normalized(); // 適当な垂直ベクトル
    Eigen::Vector3d b0 = t0.cross(n0);

    tangent.push_back(t0);
    normal.push_back(n0);
    binormal.push_back(b0);

    // フレームを各セグメントに伝播
    for (size_t i = 1; i < vertices.size() - 1; ++i) {
        Eigen::Vector3d t_curr = (vertices[i + 1].position - vertices[i].position).normalized();

        // 並進
        double dot_product = tangent[i - 1].dot(t_curr);
        double angle = acos(std::clamp(dot_product, -1.0, 1.0)); // ドット積をクランプして角度を計算

        Eigen::Vector3d axis = tangent[i - 1].cross(t_curr);
        Eigen::Matrix3d rotation;
        if (axis.norm() < 1e-10) {
            rotation = Matrix3d::Identity();
        } else {
            rotation = AngleAxisd(angle, axis.normalized()).toRotationMatrix();
        }

        Eigen::Vector3d n_curr = rotation * normal[i - 1];
        Eigen::Vector3d b_curr = t_curr.cross(n_curr);

        tangent.push_back(t_curr);
        normal.push_back(n_curr);
        binormal.push_back(b_curr);
    }
}

void DER::initialize(const HairFile& hairFile) {
    unsigned int offset = 0;
    const HairFile::Header& header = hairFile.GetHeader();

    for (unsigned int i = 0; i < header.hair_count; ++i) {
        unsigned int segmentCount = hairFile.GetSegmentsArray().empty() 
            ? header.d_segments 
            : hairFile.GetSegmentsArray()[i];

        Strand strand;

        for (unsigned int j = 0; j < segmentCount + 1; ++j) {
            strand.vertices.emplace_back(Eigen::Vector3d(
                hairFile.GetPointsArray()[(offset + j) * 3], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 1], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 2]
            ));
        }

        // ストランドを再分割
        strand.SubdivideStrand(48);

        // フレームを計算
        strand.CalculateFrame();

        strands.emplace_back(std::move(strand));
        offset += segmentCount + 1;
    }
}

void DER::update(float deltaTime) {
    for (auto& strand : strands) {
        // 必要に応じてフレームを更新
        strand.CalculateFrame();
    }
}
