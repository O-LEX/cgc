#include "HairFile.h"
#include <iostream>
#include <string>

int main() {
    HairFile hairFile;
    
    // ファイルからヘアデータを読み込む
    try {
        hairFile.LoadFromFile(SOURCE_DIR "/model/wWavy.hair");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading file: " << e.what() << std::endl;
        return 1;
    }

    // ヘアデータを表示または操作する
    std::cout << "Hair Count: " << hairFile.GetHeader().hair_count << std::endl;
    std::cout << "Point Count: " << hairFile.GetHeader().point_count << std::endl;

    // セグメントデータにアクセス
    const std::vector<unsigned short>& segments = hairFile.GetSegmentsArray();
    if (!segments.empty()) {
        std::cout << "First Segment: " << segments[0] << std::endl;
    }

    // ポイントデータにアクセス
    const std::vector<float>& points = hairFile.GetPointsArray();
    if (!points.empty()) {
        std::cout << "First Point: (" << points[0] << ", " << points[1] << ", " << points[2] << ")" << std::endl;
    }

    // データをファイルに保存する
    try {
        hairFile.SaveToFile("output.hair");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
        return 1;
    }


}
