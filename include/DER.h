#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "HairFile.h"

class Vertex {
public:
    glm::vec3 position;
    glm::vec3 velocity;

    Vertex(const glm::vec3& pos);
};

class Edge {
public:
    std::array<glm::vec3, 3> frame;
};

class Strand {
public:
    std::vector<Vertex> vertices;
    void SubdivideStrand(int subdivisions);
};

class DER {
public:
    std::vector<Strand> strands;

    void initialize(const HairFile& hairFile);
    void update(float deltaTime);
};