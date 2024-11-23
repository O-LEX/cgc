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

std::vector<Eigen::Vector3d> DERStrand::CalculateTangent() {
    std::vector<Eigen::Vector3d> tangents;
    for (unsigned i = 0; i < vertices.size() - 1; ++i) {
        tangents.emplace_back((vertices[i + 1].position - vertices[i].position).normalized());
    }
    return tangents;
}

void DERStrand::UpdateReferenceFrame() {
    std::vector<Eigen::Vector3d> cur_tangents = CalculateTangent();
    for (unsigned i = 0; i < tangents.size(); ++i) {
        Matrix3d Projection = ParallelTransport(tangents[i], cur_tangents[i]);
        ref_dir_1.emplace_back(Projection * ref_dir_1.back());
        ref_dir_2.emplace_back(Projection * ref_dir_2.back());
    }
}

void DER::initialize(const HairFile& hairFile) {
    unsigned int offset = 0;
    const HairFile::Header& header = hairFile.GetHeader();

    for (unsigned int i = 0; i < header.hair_count; ++i) {
        unsigned int segmentCount = hairFile.GetSegmentsArray().empty() 
            ? header.d_segments 
            : hairFile.GetSegmentsArray()[i];

        DERStrand strand;

        for (unsigned int j = 0; j < segmentCount + 1; ++j) {
            strand.vertices.emplace_back(Eigen::Vector3d(
                hairFile.GetPointsArray()[(offset + j) * 3], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 1], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 2]
            ));
        }

        // ストランドを再分割
        strand.SubdivideStrand(48);

        strands.emplace_back(std::move(strand));
        offset += segmentCount + 1;
    }
}
