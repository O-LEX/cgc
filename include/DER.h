#pragma once

#include <Eigen/Dense>
#include <vector>
#include "HairFile.h"
#include "Strand.h"

class DERStrand : public Strand {
public:
    std::vector<Eigen::Vector3d> tangents; // Strandの接戦

    // reference directors
    std::vector<Eigen::Vector3d> ref_dir_1;
    std::vector<Eigen::Vector3d> ref_dir_2;

    std::vector<Eigen::Vector3d> CalculateTangent(); // 接戦の計算
    void UpdateReferenceFrame(); // 参照フレームの更新
};

class DER {
public:
    std::vector<DERStrand> strands;

    void initialize(const HairFile& hairFile);
    void update(float deltaTime);
};
