/*
 * Generates a hilly terrain using perlin noise
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <littleVulkanEngine/core/lve_device.hpp>
#include <littleVulkanEngine/core/lve_model.hpp>

// std
#include <memory>

namespace lve {
using HeightMap = std::vector<std::vector<float>>;

class Terrain {
 public:
  Terrain(LveDevice& device);
  std::unique_ptr<LveModel> model{};

 private:
  void generateTerrainModel(int vertexCount, float size, float textureScaleFactor);

  static HeightMap generateHeightMap(int vertexCount, float size, float minHeight, float maxHeight);
  static glm::vec3 calculateNormal(const HeightMap& heightMap, float delta, int i, int k);
  static glm::vec4 calculateTangent(const HeightMap& heightMap, int i, int k);

  static constexpr float SIZE = 800.0f;
  static constexpr int VERTEX_COUNT = 120;

  float x;
  float z;
  LveDevice& device_;
};

}  // namespace lve
