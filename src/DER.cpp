#include "DER.h"

Vertex::Vertex(const glm::vec3& pos) : position(pos), velocity(0.0f) {}

// Function to calculate distance between two glm::vec3 points
float Distance(const glm::vec3& a, const glm::vec3& b) {
    return glm::length(b - a);
}

// Function to subdivide a strand into equal length segments
void Strand::SubdivideStrand(int subdivisions) {
    if (vertices.size() < 2) return;

    // Calculate total length
    float totalLength = 0.0f;
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        totalLength += Distance(vertices[i].position, vertices[i + 1].position);
    }

    // Determine target segment length
    float targetLength = totalLength / subdivisions;
    std::vector<Vertex> newVertices;
    newVertices.emplace_back(vertices.front());

    float accumulated = 0.0f;
    size_t current = 0;

    while (newVertices.size() < static_cast<size_t>(subdivisions) && current < vertices.size() - 1) {
        float segmentLength = Distance(vertices[current].position, vertices[current + 1].position);
        if (accumulated + segmentLength >= targetLength) {
            float remaining = targetLength - accumulated;
            float t = remaining / segmentLength;
            glm::vec3 newPos = glm::mix(vertices[current].position, vertices[current + 1].position, t);
            newVertices.emplace_back(newPos);
            // Insert the new vertex into the current segment
            vertices.insert(vertices.begin() + current + 1, Vertex(newPos));
            accumulated = 0.0f;
        } else {
            accumulated += segmentLength;
            ++current;
        }
    }

    // Ensure the last vertex is included
    newVertices.emplace_back(vertices.back().position);
    vertices = std::move(newVertices);
}

void DER::initialize(const HairFile& hairFile) {
    unsigned int offset = 0;
    const HairFile::Header& header = hairFile.GetHeader();

    for (unsigned int i = 0; i < header.hair_count; ++i) {
        unsigned int segmentCount = hairFile.GetSegmentsArray().empty() 
            ? header.d_segments 
            : hairFile.GetSegmentsArray()[i];

        Strand strand;

        for (unsigned int j = 0; j < segmentCount + 1; ++j) {
            strand.vertices.emplace_back(glm::vec3(
                hairFile.GetPointsArray()[(offset + j) * 3], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 1], 
                hairFile.GetPointsArray()[(offset + j) * 3 + 2]
            ));
        }

        // Subdivide strand
        strand.SubdivideStrand(48);

        strands.emplace_back(std::move(strand));
        offset += segmentCount + 1;
    }
}

void DER::update(float deltaTime) {
    for (auto& strand : strands) {
    }
}
