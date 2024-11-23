#include "pbd.cuh"

__global__ void pbd_kernel(Particle* particles, int numParticles, float dt, int numIterations) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < numParticles) {
        // Simple gravity
        particles[i].vel.y -= 9.81f * dt;

        // Simple collision with a plane at y = 0
        if (particles[i].pos.y < 0.0f) {
            particles[i].pos.y = 0.0f;
        }


        // Basic iterative position-based solver (very simplified)
        for(int j = 0; j < numIterations; ++j){
            particles[i].pos.y = fmaxf(particles[i].pos.y, 0.0f); // fmaxfを使う
        }

        //Update velocity (very basic, no proper integration)
        particles[i].vel.x = (particles[i].pos.x - particles[i].oldPos.x) / dt;
        particles[i].vel.y = (particles[i].pos.y - particles[i].oldPos.y) / dt;
        particles[i].vel.z = (particles[i].pos.z - particles[i].oldPos.z) / dt;
        particles[i].oldPos = particles[i].pos;
    }
}

void pbd_step(Particle* particles, int numParticles, float dt, int numIterations) {
  int threadsPerBlock = 256;
  int blocksPerGrid = (numParticles + threadsPerBlock - 1) / threadsPerBlock;
  pbd_kernel<<<blocksPerGrid, threadsPerBlock>>>(particles, numParticles, dt, numIterations);
  cudaDeviceSynchronize();
}