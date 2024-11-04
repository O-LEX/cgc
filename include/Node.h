#pragma once

#include <Eigen/Dense>

class Node {
public:
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d force;

    Node();
    Node(const Eigen::Vector3d& pos);
};
