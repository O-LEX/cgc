#include "Strand.h"


Vertex::Vertex(const Eigen::Vector3d& pos, const Eigen::Vector3d& vel)
    : position(pos), velocity(vel) {}

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