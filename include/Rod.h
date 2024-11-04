#pragma once

#include <vector>
#include <Eigen/Dense>
#include "Node.h"

class Rod {
public:
    Rod(int num_nodes, double total_length, double stiffness_, double bending_stiffness_);
    
    void computeForces(const Eigen::Vector3d& gravity);
    void timeIntegrate(double dt);
    void constrain();
    void getPositions(std::vector<Eigen::Vector3d>& positions) const;

private:
    std::vector<Node> nodes;
    double rest_length;
    double stiffness;
    double bending_stiffness;
};
