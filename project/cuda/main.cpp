#include "HairLoader.h"
#include "HairModel.h"

int main(){
    HairLoader loader;
    HairModel model;
    std::string err, warn;

    bool ret = loader.LoadFromFile(&model, &err, &warn, MODEL_DIR "/straight.hair");
    if (!ret) {
        std::cerr << "Error: " << err << std::endl;
        if (!warn.empty()) {
            std::cerr << "Warning: " << warn << std::endl;
        }
    } else {
        std::cout << "Hair model loaded successfully." << std::endl;
    }

    return 0;
}