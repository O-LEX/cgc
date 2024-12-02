#include "HairModel.h"

void HairModel::PrintData() const {
    std::cout << "Hair Count: " << hair_count << std::endl;
    std::cout << "Point Count: " << point_count << std::endl;
    std::cout << "Arrays: " << arrays << std::endl;
    std::cout << "Default Segments: " << d_segments << std::endl;
    std::cout << "Default Thickness: " << d_thickness << std::endl;
    std::cout << "Default Transparency: " << d_transparency << std::endl;
    std::cout << "Default Color: [" << d_color[0] << ", " << d_color[1] << ", " << d_color[2] << "]" << std::endl;

    std::cout << std::endl;
}