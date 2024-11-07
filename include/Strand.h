#pragma once

#include <vector>
#include <Eigen/Dense>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <iostream>
#include "Shader.h"

class Strand {
public:
    Strand();
    ~Strand();

    void setVertices(const std::vector<Eigen::Vector3d>& vertices);

    void Draw(Shader& shader);

private:
    unsigned int VAO, VBO;
    std::vector<Eigen::Vector3d> vertices;
};
