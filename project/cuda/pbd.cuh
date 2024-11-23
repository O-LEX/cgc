#ifndef PBD_CUH
#define PBD_CUH

#include <cuda_runtime.h>

// Do NOT define float3 here! Use CUDA's built-in version.

struct Particle {
    float3 pos;
    float3 oldPos;
    float3 vel;
};

void pbd_step(Particle* particles, int numParticles, float dt, int numIterations);

#endif