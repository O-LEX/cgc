#include "DER.h"

Node::Node(const glm::vec3& pos) : position(pos), velocity(0.0f), force(0.0f) {}

Strand::Strand(int nodeCount, const glm::vec3& startPosition, float nodeLength) : nodeLength(nodeLength) {
    for (int i = 0; i < nodeCount; ++i) {
        glm::vec3 pos = startPosition + glm::vec3(0.0f, -i * nodeLength, 0.0f);
        nodes.emplace_back(pos);
    }
    stiffness = 1.0f;
    damping = 0.1f;
}

void Strand::applyForces() {
    // 力の計算（弾性力、重力、ダンピングなど）
    for (size_t i = 0; i < nodes.size(); ++i) {
        // 重力
        nodes[i].force = glm::vec3(0.0f, -9.81f, 0.0f);

        // ダンピング
        nodes[i].force -= damping * nodes[i].velocity;

        // 弾性力（隣接するノードとのバネ力）
        if (i > 0) {
            glm::vec3 delta = nodes[i].position - nodes[i - 1].position;
            float length = glm::length(delta);
            glm::vec3 direction = glm::normalize(delta);
            nodes[i].force -= stiffness * (length - nodeLength) * direction;
            nodes[i - 1].force += stiffness * (length - nodeLength) * direction;
        }
    }
}

void Strand::integrate(float deltaTime) {
    // 力に基づいて位置と速度を更新
    for (auto& node : nodes) {
        node.velocity += node.force * deltaTime;
        node.position += node.velocity * deltaTime;
    }
}

void Strand::handleCollisions() {
    // 衝突処理（必要に応じて実装）
}

void Hair::initialize(const HairFile& hairFile) {
    unsigned int offset = 0;
    const HairFile::Header& header = hairFile.GetHeader();

    for (unsigned int i = 0; i < header.hair_count; ++i) {
        unsigned int segmentCount = hairFile.GetSegmentsArray().empty() 
            ? header.d_segments 
            : hairFile.GetSegmentsArray()[i];

        glm::vec3 startPos(
            hairFile.GetPointsArray()[offset * 3], 
            hairFile.GetPointsArray()[offset * 3 + 1], 
            hairFile.GetPointsArray()[offset * 3 + 2]
        );

        Strand strand(segmentCount + 1, startPos, 1.0f);

        for (unsigned int j = 0; j <= segmentCount; ++j) {
            strand.nodes[j].position = glm::vec3(
                hairFile.GetPointsArray()[(offset + j) * 3], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 1], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 2]
            );
        }

        strands.push_back(strand);
        offset += segmentCount + 1;
    }
}

void Hair::update(float deltaTime) {
    for (auto& strand : strands) {
        strand.applyForces();
        strand.integrate(deltaTime);
        strand.handleCollisions();
    }
}
