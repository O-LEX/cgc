#include <Eigen/Dense>
#include <vector>
#include "Strand.h"
#include "HairFile.h"

class PBDStrand : public Strand {
public:
    // メソッドの宣言
    void applyForces(const Eigen::Vector3d& gravity, float deltaTime);
    void integrate(float deltaTime);
    void solveConstraints(int iterations);

    // 初期距離を設定するメソッド
    void setDesiredLengths();

private:
    // 各セグメントの初期距離を保持
    std::vector<double> desiredLengths;
};

class PBD {
public:
    std::vector<PBDStrand> strands;

    void initialize(const HairFile& hairFile);
    void update(float deltaTime);

private:
    Eigen::Vector3d gravity = Eigen::Vector3d(0.0, -9.81, 0.0);
    int constraintIterations = 10;
};