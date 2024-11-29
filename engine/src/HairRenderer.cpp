#include "HairRenderer.h"

HairRenderer::~HairRenderer() {
    for (const auto& pair : vaoMap) {
        deleteVAOData(pair.second);
    }
}

void HairRenderer::CreateVAO(const HairModel& model) {
    VAOData data;
    glGenVertexArrays(1, &data.VAO);
    glGenBuffers(1, &data.VBO);

    glBindVertexArray(data.VAO);

    // Points
    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBufferData(GL_ARRAY_BUFFER, model.points.size() * sizeof(float), model.points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Thickness
    if (!model.thickness.empty()) {
        glGenBuffers(1, &data.TBO);
        glBindBuffer(GL_ARRAY_BUFFER, data.TBO);
        glBufferData(GL_ARRAY_BUFFER, model.thickness.size() * sizeof(float), model.thickness.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // Transparency
    if (!model.transparency.empty()) {
        glGenBuffers(1, &data.TrBO);
        glBindBuffer(GL_ARRAY_BUFFER, data.TrBO);
        glBufferData(GL_ARRAY_BUFFER, model.transparency.size() * sizeof(float), model.transparency.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    // Color
    if (!model.colors.empty()) {
        glGenBuffers(1, &data.CBO);
        glBindBuffer(GL_ARRAY_BUFFER, data.CBO);
        glBufferData(GL_ARRAY_BUFFER, model.colors.size() * sizeof(float), model.colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vaoMap[&model] = data;
    currentModel = &model;
}

void HairRenderer::Draw(Shader& shader) const {
    if (!currentModel) {
        return; // currentModelが設定されていない場合は描画しない
    }

    auto it = vaoMap.find(currentModel);
    if (it == vaoMap.end()) {
        return; // VAOが存在しない場合は描画しない
    }

    const VAOData& data = it->second;
    const HairModel& model = *currentModel;

    shader.setInt("useDefaultThickness", model.thickness.empty());
    shader.setInt("useDefaultTransparency", model.transparency.empty());
    shader.setInt("useDefaultColor", model.colors.empty());
    shader.setFloat("defaultThickness", model.d_thickness);
    shader.setFloat("defaultTransparency", model.d_transparency);
    shader.setVec3("defaultColor", glm::vec3(model.d_color[0], model.d_color[1], model.d_color[2]));
    
    glBindVertexArray(data.VAO);

    unsigned int offset = 0;
    for (unsigned int i = 0; i < model.hair_count; ++i) {
        unsigned int segmentCount = !model.segments.empty() ? model.segments[i] : model.d_segments;
        glDrawArrays(GL_LINE_STRIP, offset, segmentCount + 1); // segmentは幾つに分かれているかということ。ポイントの数は+1
        offset += segmentCount + 1; // オフセットを更新する際に、ポイントの数を考慮する
    }

    glBindVertexArray(0);
}

void HairRenderer::deleteVAOData(const VAOData& data) {
    glDeleteBuffers(1, &data.VBO);
    glDeleteBuffers(1, &data.TBO);
    glDeleteBuffers(1, &data.TrBO);
    glDeleteBuffers(1, &data.CBO);
    glDeleteVertexArrays(1, &data.VAO);
}