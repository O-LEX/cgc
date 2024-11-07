#include "Strand.h"

Strand::Strand()
    : VAO(0), VBO(0) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // VAOのバインド
    glBindVertexArray(VAO);

    // VBOのバインド
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 初期データは空。後で更新
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // 頂点属性の設定（位置のみ）
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Eigen::Vector3d), (void*)0);
    glEnableVertexAttribArray(0);

    // VAOのバインド解除
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Strand::~Strand(){
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Strand::setVertices(const std::vector<Eigen::Vector3d>& new_vertices){
    vertices = new_vertices;

    // VBOにデータを送信
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Eigen::Vector3d), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Strand::Draw(Shader& shader) {
    shader.use();

    glBindVertexArray(VAO);

    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

    glBindVertexArray(0);
}
