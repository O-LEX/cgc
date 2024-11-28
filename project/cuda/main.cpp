#include <iostream>
#include "pbd.cuh"
#include "HairFile.h"

int main() {
  const float dt = 0.01f;
  const int numIterations = 10;

  HairFile hairFile;
  hairFile.LoadFromFile(MODEL_DIR "/wCurly.hair");
  hairFile.PrintHeader();
}