#include "Rod.h"
#include <cmath>
#include <algorithm>

// コンストラクタ
Rod::Rod(int num_nodes, double total_length, double stiffness_, double bending_stiffness_)
    : nodes(), rest_length(total_length / (num_nodes - 1)),
      stiffness(stiffness_), bending_stiffness(bending_stiffness_) {
    nodes.reserve(num_nodes);
    // 初期位置をZ軸に沿って直線配置
    for(int i = 0; i < num_nodes; ++i){
        Eigen::Vector3d pos(0.0, 0.0, i * rest_length);
        nodes.emplace_back(pos);
    }
}

// 力の計算
void Rod::computeForces(const Eigen::Vector3d& gravity) {
    // 力のリセット
    for(auto &node : nodes){
        node.force = Eigen::Vector3d::Zero();
    }

    // 重力の適用
    for(auto &node : nodes){
        node.force += gravity;
    }

    // 引張・圧縮力の計算
    for(int i = 0; i < nodes.size() - 1; ++i){
        Eigen::Vector3d delta = nodes[i+1].position - nodes[i].position;
        double current_length = delta.norm();
        Eigen::Vector3d direction;
        if(current_length > 1e-8)
            direction = delta / current_length;
        else
            direction = Eigen::Vector3d::Zero();
        double force_magnitude = stiffness * (current_length - rest_length);
        Eigen::Vector3d force = direction * force_magnitude;

        nodes[i].force += force;
        nodes[i+1].force -= force;
    }

    // 曲げ力の計算（簡易版）
    for(int i = 1; i < nodes.size() -1; ++i){
        Eigen::Vector3d v1 = nodes[i].position - nodes[i-1].position;
        Eigen::Vector3d v2 = nodes[i+1].position - nodes[i].position;

        Eigen::Vector3d v1_norm = v1.normalized();
        Eigen::Vector3d v2_norm = v2.normalized();

        // 二つのベクトルの内積から角度を求める
        double dot_prod = v1_norm.dot(v2_norm);
        // 数値的誤差を防ぐ
        dot_prod = std::max(-1.0, std::min(1.0, dot_prod));
        double angle = std::acos(dot_prod);

        // 曲げモーメント（正方向に曲げる力）
        Eigen::Vector3d normal = v1.cross(v2).normalized();
        if(normal.hasNaN()){
            normal = Eigen::Vector3d::Zero();
        }
        Eigen::Vector3d bending_force = normal * bending_stiffness * angle;

        nodes[i-1].force += bending_force;
        nodes[i+1].force += bending_force;
        nodes[i].force -= (bending_force * 2.0);
    }
}

// 時間積分（オイラー法）
void Rod::timeIntegrate(double dt) {
    for(auto &node : nodes){
        Eigen::Vector3d acceleration = node.force; // 質量=1と仮定
        node.velocity += acceleration * dt;
        node.position += node.velocity * dt;
    }
}

// 拘束条件の適用（最初の節点を固定）
void Rod::constrain(){
    if(!nodes.empty()){
        nodes[0].velocity = Eigen::Vector3d::Zero();
        nodes[0].position = Eigen::Vector3d::Zero();
    }
    // 他の拘束条件も必要に応じて追加可能
}

// ロッドの現在の位置を取得
void Rod::getPositions(std::vector<Eigen::Vector3d>& positions) const {
    positions.reserve(nodes.size());
    for(const auto &node : nodes){
        positions.emplace_back(node.position);
    }
}