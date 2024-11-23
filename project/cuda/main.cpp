#include <iostream>
#include <random> // rand()を使う場合、このヘッダーが必要になります。
#include "pbd.cuh" // CUDAカーネルのプロトタイプ宣言

int main() {
  // シミュレーションパラメータ
  const int numParticles = 1000;
  const float dt = 0.01f;
  const int numIterations = 10;

  // 乱数の初期化
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0f, 1.0f);


  // ホスト側でパーティクルを初期化
  Particle* h_particles = new Particle[numParticles];
  for (int i = 0; i < numParticles; ++i) {
    h_particles[i].pos.x = dis(gen);
    h_particles[i].pos.y = dis(gen) + 1.0f; // 平面より上に初期化
    h_particles[i].pos.z = dis(gen);
    h_particles[i].oldPos = h_particles[i].pos;
    h_particles[i].vel = {0.0f, 0.0f, 0.0f};
  }

  // デバイスメモリ確保
  Particle* d_particles;
  cudaError_t cudaStatus = cudaMalloc((void**)&d_particles, numParticles * sizeof(Particle));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    return 1;
  }

  // ホストからデバイスへのデータ転送
  cudaStatus = cudaMemcpy(d_particles, h_particles, numParticles * sizeof(Particle), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy (HtoD) failed!");
    return 1;
  }

  // シミュレーションループ
  for (int i = 0; i < 1000; ++i) {
    // 位置の更新（重力と衝突）
    pbd_step(d_particles, numParticles, dt, numIterations);
    cudaDeviceSynchronize();

    // デバイスからホストへのデータ転送
    cudaStatus = cudaMemcpy(h_particles, d_particles, numParticles * sizeof(Particle), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
      fprintf(stderr, "cudaMemcpy (DtoH) failed!");
      return 1;
    }

    // 位置の出力（テスト用）
    if (i % 100 == 0) {
      std::cout << "Iteration " << i << ": Particle 0 position: " << h_particles[0].pos.x << ", " << h_particles[0].pos.y << ", " << h_particles[0].pos.z << std::endl;
    }
  }

  // メモリ解放
  cudaFree(d_particles);
  delete[] h_particles;

  return 0;
}