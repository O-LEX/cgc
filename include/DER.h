#pragma once

#include <Eigen/Dense>
#include <vector>
#include "HairFile.h"

class Vertex {
public:
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;

    Vertex(const Eigen::Vector3d& pos);
};

class Strand {
public:
    std::vector<Vertex> vertices;

    // 各セグメントに対応するフレームを格納
    std::vector<Eigen::Vector3d> tangent;   // 接線ベクトル (d3)
    std::vector<Eigen::Vector3d> normal;    // 法線ベクトル (d1)
    std::vector<Eigen::Vector3d> binormal;  // 捩れベクトル (d2)

    std::vector<Eigen::Vector3d> ref_dir_1; // reference directors
    std::vector<Eigen::Vector3d> ref_dir_2;

    void SubdivideStrand(int subdivisions); // シミュレーションのために再分割
    void CalculateFrame();                // 空間方向のフレームを計算
    void UpdateReferenceFrame(); //Reference frameの変化を蓄積
    void UpdateMaterialFrame(); //Reference frameをtheta_jだけ回転
};

class DER {
public:
    std::vector<Strand> strands;

    void initialize(const HairFile& hairFile);
    void update(float deltaTime);
};
