/*
 * Lattices that support a variety of sampling operations
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lattice.hpp"

#include "perlin_noise.hpp"

namespace lve {

namespace generate {

GridLattice2D::GridLattice2D(int vCount) : vertexCount{vCount} {
  values = std::vector(vertexCount, std::vector<float>(vertexCount, 0.0f));
}

/**
 * Samples value using cubic weight function
 */
float GridLattice2D::sampleBilinear(glm::vec2 p) {
  // map s from (0, 1) into grid coordinate space
  p *= vertexCount;
  glm::vec2 p0 = glm::floor(p);
  glm::vec2 p1 = p0 + glm::vec2(1.0f, 0.0f);
  glm::vec2 p2 = p0 + glm::vec2(0.0f, 1.0f);
  glm::vec2 p3 = p0 + glm::vec2(1.0f, 1.0f);

  float u = p.x - p0.x;
  float v = p.y - p0.y;

  return (1.0f - u) * (1.0f - v) * nearestValue(p0) + u * (1.0f - v) * nearestValue(p1) +
         (1.0f - u) * v * nearestValue(p2) + u * v * nearestValue(p3);
}

float GridLattice2D::nearestValue(glm::vec2 p) {
  p = glm::clamp(p, glm::vec2(0.0), glm::vec2(static_cast<float>(vertexCount - 1)));
  int y = static_cast<int>(std::nearbyint(p.y));
  int x = static_cast<int>(std::nearbyint(p.x));
  return values[y][x];
}

/**
 * Defined on lattice with bounds [0,]
 */
glm::vec2 GridLattice2D::sampleGradient(glm::vec2 p) {
  p *= vertexCount;
  glm::vec2 p0 = glm::floor(p);
  glm::vec2 p1 = p0 + glm::vec2(1.0f, 0.0f);
  glm::vec2 p2 = p0 + glm::vec2(0.0f, 1.0f);
  glm::vec2 p3 = p0 + glm::vec2(1.0f, 1.0f);

  float u = p.x - p0.x;
  float v = p.y - p0.y;

  return (1.0f - u) * (1.0f - v) * nearestGradient(p0) + u * (1.0f - v) * nearestGradient(p1) +
         (1.0f - u) * v * nearestGradient(p2) + u * v * nearestGradient(p3);
}

glm::vec2 GridLattice2D::nearestGradient(glm::vec2 p) {
  p = glm::clamp(p, glm::vec2(0.0), glm::vec2(static_cast<float>(vertexCount - 1)));
  int y = static_cast<int>(std::nearbyint(p.y));
  int x = static_cast<int>(std::nearbyint(p.x));

  float left = y - 1 >= 0 ? values[y - 1][x] : values[y][x];
  float right = y + 1 < vertexCount ? values[y + 1][x] : values[y][x];
  float bottom = x - 1 >= 0 ? values[y][x - 1] : values[y][x];
  float top = x + 1 < vertexCount ? values[y][x + 1] : values[y][x];

  float dhdx = 0.5f * (top - bottom);
  float dhdy = 0.5f * (right - left);
  return {dhdx, dhdy};
}

}  // namespace generate
}  // namespace lve