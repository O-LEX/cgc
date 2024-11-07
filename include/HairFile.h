#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class HairFile {
public:
    struct Header {
        char signature[4];
        unsigned int hair_count = 0;
        unsigned int point_count = 0;
        unsigned int arrays = 0;
        unsigned int d_segments = 0;
        float d_thickness = 1.0f;
        float d_transparency = 0.0f;
        float d_color[3] = {1.0f, 1.0f, 1.0f};
        char info[88] = {0};
    };

    HairFile() = default;
    ~HairFile() = default;

    const Header& GetHeader() const { return header; }
    const std::vector<unsigned short>& GetSegmentsArray() const { return segments; }
    const std::vector<float>& GetPointsArray() const { return points; }
    const std::vector<float>& GetThicknessArray() const { return thickness; }
    const std::vector<float>& GetTransparencyArray() const { return transparency; }
    const std::vector<float>& GetColorsArray() const { return colors; }

    void SetHairCount(int count);
    void SetPointCount(int count);

    void LoadFromFile(const std::string& filename);
    void SaveToFile(const std::string& filename) const;

    void CreateVAO();
    void Draw() const;
    void PrintHeader() const;

private:
    Header header;
    std::vector<unsigned short> segments;
    std::vector<float> points;
    std::vector<float> thickness;
    std::vector<float> transparency;
    std::vector<float> colors;

    GLuint VAO, VBO;

    static constexpr int HAIR_FILE_SEGMENTS_BIT = 1;
    static constexpr int HAIR_FILE_POINTS_BIT = 2;
    static constexpr int HAIR_FILE_THICKNESS_BIT = 4;
    static constexpr int HAIR_FILE_TRANSPARENCY_BIT = 8;
    static constexpr int HAIR_FILE_COLORS_BIT = 16;
};
