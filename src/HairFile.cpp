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

    glBindVertexArray(VAO);

    // Points
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Thickness
    if (header.arrays & HAIR_FILE_THICKNESS_BIT) {
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, thickness.size() * sizeof(float), thickness.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    // Transparency
    if (header.arrays & HAIR_FILE_TRANSPARENCY_BIT) {
        glGenBuffers(1, &TrBO);
        glBindBuffer(GL_ARRAY_BUFFER, TrBO);
        glBufferData(GL_ARRAY_BUFFER, transparency.size() * sizeof(float), transparency.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    // Color
    if (header.arrays & HAIR_FILE_COLORS_BIT) {
        glGenBuffers(1, &CBO);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void HairFile::Draw(Shader& shader) const {
    shader.setInt("useDefaultThickness", !(header.arrays & HAIR_FILE_THICKNESS_BIT));
    shader.setInt("useDefaultTransparency", !(header.arrays & HAIR_FILE_TRANSPARENCY_BIT));
    shader.setInt("useDefaultColor", !(header.arrays & HAIR_FILE_COLORS_BIT));
    shader.setFloat("defaultThickness", header.d_thickness);
    shader.setFloat("defaultTransparency", header.d_transparency);
    shader.setVec3("defaultColor", glm::vec3(header.d_color[0], header.d_color[1], header.d_color[2]));
    
    glBindVertexArray(VAO);

    unsigned int offset = 0;
    for (unsigned int i = 0; i < header.hair_count; ++i) {
        unsigned int segmentCount = (header.arrays & HAIR_FILE_SEGMENTS_BIT) ? segments[i] : header.d_segments;
        glDrawArrays(GL_LINE_STRIP, offset, segmentCount + 1); // segmentは幾つに分かれているかということ。ポイントの数は+1
        offset += segmentCount + 1; // オフセットを更新する際に、ポイントの数を考慮する
    }

    glBindVertexArray(0);
}

void HairFile::PrintHeader() const {
    std::cout << "Header Information:" << std::endl;
    std::cout << "Signature: " << std::string(header.signature, 4) << std::endl;
    std::cout << "Hair Count: " << header.hair_count << std::endl;
    std::cout << "Point Count: " << header.point_count << std::endl;
    std::cout << "Arrays: " << header.arrays << " (";
    if (header.arrays & HAIR_FILE_SEGMENTS_BIT) std::cout << "Segments ";
    if (header.arrays & HAIR_FILE_POINTS_BIT) std::cout << "Points ";
    if (header.arrays & HAIR_FILE_THICKNESS_BIT) std::cout << "Thickness ";
    if (header.arrays & HAIR_FILE_TRANSPARENCY_BIT) std::cout << "Transparency ";
    if (header.arrays & HAIR_FILE_COLORS_BIT) std::cout << "Colors ";
    std::cout << ")" << std::endl;
    std::cout << "Default Segments: " << header.d_segments << std::endl;
    std::cout << "Default Thickness: " << header.d_thickness << std::endl;
    std::cout << "Default Transparency: " << header.d_transparency << std::endl;
    std::cout << "Default Color: (" << header.d_color[0] << ", " << header.d_color[1] << ", " << header.d_color[2] << ")" << std::endl;
    std::cout << "Info: " << std::string(header.info, 88) << std::endl;
}