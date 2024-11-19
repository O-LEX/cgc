#pragma once

#include <Eigen/Dense>
#include <vector>

class Vertex {
public:
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;

    Vertex(const Eigen::Vector3d& pos, const Eigen::Vector3d& vel = Eigen::Vector3d::Zero());
};

class Strand {
public:
    int vertexCount;
    std::vector<Vertex> vertices;

    Strand(const std::vector<Vertex>& vertices, int vertexCount = 10);

    void SubdivideStrand(); // シミュレーションのために再分割
    std::vector<Eigen::Vector3d> getPositions() const; // 位置を取得
};