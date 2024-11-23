#include "PBD.h"
#include <Eigen/Dense>
#include <cmath>

PBDStrand::PBDStrand(const std::vector<Vertex>& vertices, int vertexCount)
    : Strand(vertices, vertexCount) // 基底クラスのコンストラクタを呼び出す
{
    setDesiredLengths();
}

// Apply external forces (e.g., gravity) to each vertex's velocity
void PBDStrand::applyForces(const Eigen::Vector3d& gravity, float deltaTime) {
    for (auto& vertex : vertices) {
        vertex.velocity += gravity * deltaTime;
    }
}

// Integrate positions based on current velocities
void PBDStrand::integrate(float deltaTime) {
    for (auto& vertex : vertices) {
        vertex.position += vertex.velocity * deltaTime;
    }
}

// Solve distance constraints to maintain desired segment lengths
void PBDStrand::solveConstraints(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        for (size_t j = 0; j < vertices.size() - 1; ++j) {
            Vertex& v1 = vertices[j];
            Vertex& v2 = vertices[j + 1];

            Eigen::Vector3d delta = v2.position - v1.position;
            double currentLength = delta.norm();

            if (currentLength == 0.0) continue;

            double desiredLength = desiredLengths[j];
            Eigen::Vector3d correction = delta * (1.0 - desiredLength / currentLength) * 0.5;
            v1.position += correction;
            v2.position -= correction;
        }
    }
}

// 計算した初期距離を設定
void PBDStrand::setDesiredLengths() {
    desiredLengths.clear();
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        double length = (vertices[i + 1].position - vertices[i].position).norm();
        desiredLengths.push_back(length);
    }
}

void PBD::initialize(const HairFile& hairFile) {
    unsigned int offset = 0;
    const HairFile::Header& header = hairFile.GetHeader();

    for (unsigned int i = 0; i < header.hair_count; ++i) {
        unsigned int segmentCount = hairFile.GetSegmentsArray().empty() 
            ? header.d_segments 
            : hairFile.GetSegmentsArray()[i];

        std::vector<Vertex> vertices;
        for (unsigned int j = 0; j < segmentCount + 1; ++j) {
            Eigen::Vector3d position(
                hairFile.GetPointsArray()[(offset + j) * 3], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 1], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 2]
            );
            Vertex v(position);
            vertices.push_back(v);
        }

        PBDStrand strand(vertices, vertexCount);
        strands.emplace_back(std::move(strand));
        offset += segmentCount + 1;
    }
}

// Update PBD simulation by applying forces, integrating positions, and solving constraints
void PBD::update(float deltaTime) {

    for (auto& strand : strands) {
        strand.applyForces(gravity, deltaTime);
        strand.integrate(deltaTime);
        strand.solveConstraints(constraintIterations);
    }
    updatePoints();
    updateVBO();
}

void PBD::updatePoints() {
    points.clear();
    for (const auto& strand : strands) {
        std::vector<Eigen::Vector3d> positions = strand.getPositions();
        for (const auto& pos : positions) {
            points.push_back(static_cast<float>(pos.x()));
            points.push_back(static_cast<float>(pos.y()));
            points.push_back(static_cast<float>(pos.z()));
        }
    }
}

void PBD::CreateVAO() {
    updatePoints();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PBD::updateVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(float), points.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PBD::draw(Shader& shader) const {
    glBindVertexArray(VAO);

    unsigned int offset = 0;
    for (const auto& strand : strands) {
        glDrawArrays(GL_LINE_STRIP, offset, vertexCount);
        offset += vertexCount;
    }

    glBindVertexArray(0);
}