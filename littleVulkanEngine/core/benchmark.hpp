#pragma once

#include <chrono>
#include <iostream>

// move this to something like debug
namespace lve {
namespace benchmark {
void countFrames() {
  static float totalSeconds = 0.0f;
  static int totalFrames = 0;
  static int frameCount = 0;
  static auto startTime = std::chrono::high_resolution_clock::now();
  auto currentTime = std::chrono::high_resolution_clock::now();
  float time =
      std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

  frameCount += 1;
  if (time > 1) {
    totalFrames += frameCount;
    totalSeconds += time;
    std::cout << "FPS: " << frameCount << '\n';
    std::cout << "AVG: " << totalFrames / totalSeconds << "\n\n";
    frameCount = 0;
    startTime = currentTime;
  }
}
}  // namespace benchmark
}  // namespace lve
