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
    std::vector<Vertex> vertices;

    void SubdivideStrand(int subdivisions); // シミュレーションのために再分割
};