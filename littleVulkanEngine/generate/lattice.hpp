/*
 * Lattices that support a variety of sampling operations
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // open gl uses -1 to 1, vk is 0 to 1
#include <glm/glm.hpp>

// std
#include <vector>

namespace lve {

namespace generate {

class GridLattice2D {
 public:
  GridLattice2D(int vertexCount);

  float nearestValue(glm::vec2 p);
  float sampleBilinear(glm::vec2 p);
  glm::vec2 sampleGradient(glm::vec2 p);
  glm::vec2 nearestGradient(glm::vec2 p);

  std::vector<std::vector<float>> values;

  const int vertexCount;
};
}  // namespace generate

}  // namespace lve