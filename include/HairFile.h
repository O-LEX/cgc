// cyCodeBase by Cem Yuksel
// [www.cemyuksel.com]
//-------------------------------------------------------------------------------
//! \file   cyHairFile.h 
//! \author Cem Yuksel
//! 
//! \brief  A class for the HAIR file type
//! 
//-------------------------------------------------------------------------------
//
// Copyright (c) 2016, Cem Yuksel <cem@cemyuksel.com>
// All rights reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal 
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// 
//-------------------------------------------------------------------------------

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
