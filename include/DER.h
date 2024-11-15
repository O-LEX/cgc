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

class Strand {
public:
    std::vector<Vertex> vertices;

    // 各セグメントに対応するフレームを格納
    std::vector<glm::vec3> tangent;   // 接線ベクトル (d3)
    std::vector<glm::vec3> normal;   // 法線ベクトル (d1)
    std::vector<glm::vec3> binormal; // 捩れベクトル (d2)

    void SubdivideStrand(int subdivisions); // シミュレーションのために再分割
    void CalculateFrames();                // 空間方向のフレームを計算
};

class DER {
public:
    std::vector<Strand> strands;

    void initialize(const HairFile& hairFile);
    void update(float deltaTime);
};
