#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "HairFile.h"

class Node {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;

    Node(const glm::vec3& pos);
};

class Strand {
public:
    std::vector<Node> nodes;
    float stiffness;
    float damping;
    float nodeLength;

    Strand(int nodeCount, const glm::vec3& startPosition, float nodeLength);

    void applyForces();
    void integrate(float deltaTime);
    void handleCollisions();
};

class Hair {
public:
    std::vector<Strand> strands;

    void initialize(const HairFile& hairFile);
    void update(float deltaTime);
};