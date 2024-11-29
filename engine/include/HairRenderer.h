#pragma once

#include "HairModel.h"
#include "Shader.h"
#include <glad/gl.h>
#include <unordered_map>

class HairRenderer {
public:
    HairRenderer() = default;
    ~HairRenderer();

    void CreateVAO(const HairModel& model);
    void Draw(Shader& shader) const;

private:
    struct VAOData {
        GLuint VAO;
        GLuint VBO;
        GLuint TBO;
        GLuint TrBO;
        GLuint CBO;
    };

    std::unordered_map<const HairModel*, VAOData> vaoMap;
    const HairModel* currentModel = nullptr;

    void deleteVAOData(const VAOData& data);
};