/*
 * Functions to generate various model shapes
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "shapes.hpp"

#include <littleVulkanEngine/core/lve_model.hpp>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // open gl uses -1 to 1, vk is 0 to 1
#include <glm/glm.hpp>

// std
#include <memory>

namespace lve {

namespace generate {

// std::unique_ptr<LveModel::Builder> quad(
//     glm::vec3 center, glm::vec3 normal, float sideLength = 1.0f) {
//   auto builder = std::make_unique<LveModel::Builder>();

//   auto n = glm::normalize(normal);

//   return builder;
// }

}  // namespace generate
}  // namespace lve