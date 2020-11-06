/*
 * Simple perlin noise implementation
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "perlin_noise.hpp"

// std
#include <cstdlib>

namespace lve {

namespace generate {

float randFloat() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }

float cubicWeight(float t) {
  float a = glm::abs(t);
  if (a < 1.0f) {
    return 2.0f * a * a * a - 3.0f * a * a + 1;
  }
  return 0;
}

Perlin::Perlin(unsigned int generatorSeed) {
  srand(generatorSeed);
  randVecs.reserve(permutations.size());
  for (int i = 0; i < permutations.size(); i++) {
    randVecs.push_back(genRandUnitSphereVec());
  }
}

glm::vec3 Perlin::genRandUnitSphereVec() {
  glm::vec3 randSphereVec{};
  do {
    randSphereVec.x = 2.0f * randFloat() - 1.0f;
    randSphereVec.y = 2.0f * randFloat() - 1.0f;
    randSphereVec.z = 2.0f * randFloat() - 1.0f;
  } while (glm::dot(randSphereVec, randSphereVec) >= 1);
  return glm::normalize(randSphereVec);
}

glm::vec3 Perlin::getPseudoRand(int i, int j, int k) {
  int pk = permutations[k % permutations.size()];
  int pj = permutations[(j + pk) % permutations.size()];
  int pi = permutations[(i + pj) % permutations.size()];
  return randVecs[pi];
}

float Perlin::weightedNoise(glm::vec3 v, int i, int j, int k) {
  float dotProd = glm::dot(v, getPseudoRand(i, j, k));
  return cubicWeight(v.x) * cubicWeight(v.y) * cubicWeight(v.z) * dotProd;
}

float Perlin::noise(glm::vec3 v) {
  int i = static_cast<int>(v.x);
  int j = static_cast<int>(v.y);
  int k = static_cast<int>(v.z);
  return weightedNoise(v - glm::vec3{i, j, k}, i, j, k) +
         weightedNoise(v - glm::vec3{i, j, k + 1}, i, j, k + 1) +
         weightedNoise(v - glm::vec3{i, j + 1, k}, i, j + 1, k) +
         weightedNoise(v - glm::vec3{i, j + 1, k + 1}, i, j + 1, k + 1) +
         weightedNoise(v - glm::vec3{i + 1, j, k}, i + 1, j, k) +
         weightedNoise(v - glm::vec3{i + 1, j, k + 1}, i + 1, j, k + 1) +
         weightedNoise(v - glm::vec3{i + 1, j + 1, k}, i + 1, j + 1, k) +
         weightedNoise(v - glm::vec3{i + 1, j + 1, k + 1}, i + 1, j + 1, k + 1);
}

/*
 * Generates perlin noise
 * v: A vector with components typically in the (0, 1) range. Values outside this range effectively
 * act as a scaling factor that applies a zoom zoom: specifies the zoom level, a value of 1 is
 * default zoom level
 *
 * octaves: The number of octaves, i.e scaled copies applied to the noise
 *
 * lacunarity: effectivly adjusts the frequency of each subsequent octave. Default value is 2,
 * meaning that each octave would have twice the varibility over space
 *
 * persistence: adjusts amplitude, default value is 0.5. So each octave subsequent octave
 * contributes half as strongly to the overall shape
 *
 * So for example, large rolling mountains that have a sandpaper like surface would have a high
 * lacunarity and low persistence
 */
float Perlin::perlinNoise(
    glm::vec3 v, int octaves, float lacunarity = 2.0f, float persistence = 0.5f) {
  float lacunarityMultiplier = 1.0f;
  float persistenceMultiplier = 1.0f;
  float totalNoise = 0;
  for (int i = 0; i < octaves; i++) {
    totalNoise += persistenceMultiplier * noise(lacunarityMultiplier * v);
    lacunarityMultiplier *= lacunarity;
    persistenceMultiplier *= persistence;
  }

  return totalNoise;
}

}  // namespace generate
}  // namespace lve
