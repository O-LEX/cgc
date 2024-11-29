#include "HairLoader.h"
#include <fstream>

bool HairLoader::LoadFromFile(HairModel* model, std::string* err, std::string* warn, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
    if (file.gcount() < sizeof(Header)) {
        if (err) *err = "Failed to read header";
        return false;
    }

    // Convert signature to std::string for comparison
    std::string signature(header.signature, 4);
    if (signature != "HAIR") {
        if (err) *err = "Invalid file signature";
        return false;
    }

    model->hair_count = header.hair_count;
    model->point_count = header.point_count;
    model->arrays = header.arrays;
    model->d_segments = header.d_segments;
    model->d_thickness = header.d_thickness;
    model->d_transparency = header.d_transparency;
    std::copy(std::begin(header.d_color), std::end(header.d_color), std::begin(model->d_color));

    if (header.arrays & HAIR_FILE_SEGMENTS_BIT) {
        model->segments.resize(header.hair_count);
        file.read(reinterpret_cast<char*>(model->segments.data()), header.hair_count * sizeof(unsigned short));
    }
    if (header.arrays & HAIR_FILE_POINTS_BIT) {
        model->points.resize(header.point_count * 3);
        file.read(reinterpret_cast<char*>(model->points.data()), header.point_count * 3 * sizeof(float));
    }
    if (header.arrays & HAIR_FILE_THICKNESS_BIT) {
        model->thickness.resize(header.point_count);
        file.read(reinterpret_cast<char*>(model->thickness.data()), header.point_count * sizeof(float));
    }
    if (header.arrays & HAIR_FILE_TRANSPARENCY_BIT) {
        model->transparency.resize(header.point_count);
        file.read(reinterpret_cast<char*>(model->transparency.data()), header.point_count * sizeof(float));
    }
    if (header.arrays & HAIR_FILE_COLORS_BIT) {
        model->colors.resize(header.point_count * 3);
        file.read(reinterpret_cast<char*>(model->colors.data()), header.point_count * 3 * sizeof(float));
    }

    if (!file) {
        if (err) *err = "Error reading data arrays";
        return false;
    }

    return true;
}