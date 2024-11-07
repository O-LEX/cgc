#include "HairFile.h"
#include <fstream>

void HairFile::SetHairCount(int count) {
    header.hair_count = count;
    segments.resize(count);
}

void HairFile::SetPointCount(int count) {
    header.point_count = count;
    points.resize(count * 3);
    thickness.resize(count);
    transparency.resize(count);
    colors.resize(count * 3);
}

void HairFile::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file");

    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
    if (file.gcount() < sizeof(Header)) throw std::runtime_error("Failed to read header");

    // Convert signature to std::string for comparison
    std::string signature(header.signature, 4);
    if (signature != "HAIR") throw std::runtime_error("Invalid file signature");

    if (header.arrays & HAIR_FILE_SEGMENTS_BIT) {
        segments.resize(header.hair_count);
        file.read(reinterpret_cast<char*>(segments.data()), header.hair_count * sizeof(unsigned short));
    }
    if (header.arrays & HAIR_FILE_POINTS_BIT) {
        points.resize(header.point_count * 3);
        file.read(reinterpret_cast<char*>(points.data()), header.point_count * 3 * sizeof(float));
    }
    if (header.arrays & HAIR_FILE_THICKNESS_BIT) {
        thickness.resize(header.point_count);
        file.read(reinterpret_cast<char*>(thickness.data()), header.point_count * sizeof(float));
    }
    if (header.arrays & HAIR_FILE_TRANSPARENCY_BIT) {
        transparency.resize(header.point_count);
        file.read(reinterpret_cast<char*>(transparency.data()), header.point_count * sizeof(float));
    }
    if (header.arrays & HAIR_FILE_COLORS_BIT) {
        colors.resize(header.point_count * 3);
        file.read(reinterpret_cast<char*>(colors.data()), header.point_count * 3 * sizeof(float));
    }

    if (!file) throw std::runtime_error("Error reading data arrays");
}

void HairFile::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file");

    file.write(reinterpret_cast<const char*>(&header), sizeof(Header));
    if (header.arrays & HAIR_FILE_SEGMENTS_BIT) file.write(reinterpret_cast<const char*>(segments.data()), header.hair_count * sizeof(unsigned short));
    if (header.arrays & HAIR_FILE_POINTS_BIT) file.write(reinterpret_cast<const char*>(points.data()), header.point_count * 3 * sizeof(float));
    if (header.arrays & HAIR_FILE_THICKNESS_BIT) file.write(reinterpret_cast<const char*>(thickness.data()), header.point_count * sizeof(float));
    if (header.arrays & HAIR_FILE_TRANSPARENCY_BIT) file.write(reinterpret_cast<const char*>(transparency.data()), header.point_count * sizeof(float));
    if (header.arrays & HAIR_FILE_COLORS_BIT) file.write(reinterpret_cast<const char*>(colors.data()), header.point_count * 3 * sizeof(float));

    if (!file) throw std::runtime_error("Error writing data arrays");
}

void HairFile::CreateVAO() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, segments.size() * sizeof(unsigned short), segments.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void HairFile::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_STRIP, segments.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}


void HairFile::PrintHeader() const {
    std::cout << "Header Information:" << std::endl;
    std::cout << "Signature: " << std::string(header.signature, 4) << std::endl;
    std::cout << "Hair Count: " << header.hair_count << std::endl;
    std::cout << "Point Count: " << header.point_count << std::endl;
    std::cout << "Arrays: " << header.arrays << std::endl;
    std::cout << "Default Segments: " << header.d_segments << std::endl;
    std::cout << "Default Thickness: " << header.d_thickness << std::endl;
    std::cout << "Default Transparency: " << header.d_transparency << std::endl;
    std::cout << "Default Color: (" << header.d_color[0] << ", " << header.d_color[1] << ", " << header.d_color[2] << ")" << std::endl;
    std::cout << "Info: " << std::string(header.info, 88) << std::endl;
}