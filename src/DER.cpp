#include "DER.h"
#include <glm/gtx/norm.hpp> // for glm::length2
#include <glm/gtx/rotate_vector.hpp> // for glm::rotate

Vertex::Vertex(const glm::vec3& pos) : position(pos), velocity(0.0f) {}

// Function to calculate distance between two glm::vec3 points
float Distance(const glm::vec3& a, const glm::vec3& b) {
    return glm::length(b - a);
}

// Function to subdivide a strand into equal length segments
void Strand::SubdivideStrand(int subdivisions) {
    if (vertices.size() < 2) return;

    // Calculate total length
    float totalLength = 0.0f;
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        totalLength += Distance(vertices[i].position, vertices[i + 1].position);
    }

    // Determine target segment length
    float targetLength = totalLength / subdivisions;
    std::vector<Vertex> newVertices;
    newVertices.emplace_back(vertices.front());

    float accumulated = 0.0f;
    size_t current = 0;

    while (newVertices.size() < static_cast<size_t>(subdivisions) && current < vertices.size() - 1) {
        float segmentLength = Distance(vertices[current].position, vertices[current + 1].position);
        if (accumulated + segmentLength >= targetLength) {
            float remaining = targetLength - accumulated;
            float t = remaining / segmentLength;
            glm::vec3 newPos = glm::mix(vertices[current].position, vertices[current + 1].position, t);
            newVertices.emplace_back(newPos);
            accumulated = 0.0f;
        } else {
            accumulated += segmentLength;
            ++current;
        }
    }

    // Ensure the last vertex is included
    newVertices.emplace_back(vertices.back().position);
    vertices = std::move(newVertices);
}

// フレームを計算する関数
void Strand::CalculateFrames() {
    if (vertices.size() < 2) return;

    tangent.clear();
    normal.clear();
    binormal.clear();

    // 初期フレームの設定
    glm::vec3 t0 = glm::normalize(vertices[1].position - vertices[0].position);
    glm::vec3 n0 = glm::normalize(glm::vec3(-t0.y, t0.x, 0.0f)); // 適当な垂直ベクトル
    glm::vec3 b0 = glm::cross(t0, n0);

    tangent.push_back(t0);
    normal.push_back(n0);
    binormal.push_back(b0);

    // フレームを各セグメントに伝播
    for (size_t i = 1; i < vertices.size() - 1; ++i) {
        glm::vec3 t_curr = glm::normalize(vertices[i + 1].position - vertices[i].position);

        // 並進
        glm::vec3 n_curr = glm::rotate(normal[i - 1], glm::acos(glm::dot(tangent[i - 1], t_curr)), glm::cross(tangent[i - 1], t_curr));
        glm::vec3 b_curr = glm::cross(t_curr, n_curr);

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
            strand.vertices.emplace_back(glm::vec3(
                hairFile.GetPointsArray()[(offset + j) * 3], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 1], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 2]
            ));
        }

        // Subdivide strand
        strand.SubdivideStrand(48);

        // フレームを計算
        strand.CalculateFrames();

        strands.emplace_back(std::move(strand));
        offset += segmentCount + 1;
    }
}

void DER::update(float deltaTime) {
    for (auto& strand : strands) {
        // 必要に応じてフレームを更新
        strand.CalculateFrames();
    }
}
