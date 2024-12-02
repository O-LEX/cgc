#pragma once

#include <vector>
#include <iostream>

class HairModel {
public:
    HairModel() = default;
    ~HairModel() = default;

    unsigned int hair_count;
    unsigned int point_count;
    unsigned int arrays;
    unsigned int d_segments;
    float d_thickness;
    float d_transparency;
    float d_color[3];
    std::vector<unsigned short> segments;
    std::vector<float> points;
    std::vector<float> thickness;
    std::vector<float> transparency;
    std::vector<float> colors;

    void PrintData() const;
};