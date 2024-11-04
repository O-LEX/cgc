#include "Node.h"

// デフォルトコンストラクタ
Node::Node()
    : position(Eigen::Vector3d::Zero()),
      velocity(Eigen::Vector3d::Zero()),
      force(Eigen::Vector3d::Zero()) {}

// 位置を指定するコンストラクタ
Node::Node(const Eigen::Vector3d& pos)
    : position(pos),
      velocity(Eigen::Vector3d::Zero()),
      force(Eigen::Vector3d::Zero()) {}