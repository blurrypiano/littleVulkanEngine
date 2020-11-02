/*
 * Camera class
 *
 * A really basic moveable camera
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "lve_window.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // open gl uses -1 to 1, vk is 0 to 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace lve {

class Camera {
 public:
  void update(GLFWwindow* window);
  glm::mat4 viewing();
  glm::mat4 perspective(float aspectRatio);

 private:
  glm::vec3 position_{0.0f, 0.0f, -10.0f};
  float roll_{};
  float yaw_{static_cast<float>(M_PI)};
  float pitch_{};
  float fovDegrees_{45.0f};
  float near_{0.1f};
  float far_{1000.0f};
  float speed_{2.0f};
};

}  // namespace lve
