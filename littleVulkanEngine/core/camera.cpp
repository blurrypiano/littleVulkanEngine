/*
 * Camera class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "camera.hpp"

#include <chrono>
#include <iostream>

namespace lve {

void Camera::update(GLFWwindow* window) {
  static auto startTime = std::chrono::high_resolution_clock::now();
  auto currentTime = std::chrono::high_resolution_clock::now();
  float delta =
      std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
  startTime = currentTime;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    delta *= 4;
  }

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    speed_ += delta;
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    speed_ = std::max(0.1f, speed_ - delta);
  }

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    position_.y += delta * speed_;
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    position_.y -= delta * speed_;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    pitch_ += delta;
    pitch_ = std::min(pitch_, 0.98f * 0.5f * static_cast<float>(M_PI));
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    pitch_ -= delta;
    pitch_ = std::max(pitch_, -0.98f * 0.5f * static_cast<float>(M_PI));
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    yaw_ += delta;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    yaw_ -= delta;
  }

  glm::vec3 lateralMotion{};
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    lateralMotion.z -= delta * speed_;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    lateralMotion.z += delta * speed_;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    lateralMotion.x -= delta * speed_;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    lateralMotion.x += delta * speed_;
  }
  position_.x += -lateralMotion.z * std::sin(yaw_) + lateralMotion.x * std::cos(yaw_);
  position_.z += lateralMotion.x * std::sin(yaw_) + lateralMotion.z * std::cos(yaw_);
}

glm::mat4 Camera::viewing() {
  auto viewing = glm::mat4(1.0);
  viewing[3] = glm::vec4(-position_, 1.0f);
  auto r1 = glm::rotate(glm::mat4(1.0), pitch_, glm::vec3(1.0, 0.0, 0.0));
  auto r2 = glm::rotate(glm::mat4(1.0), yaw_, glm::vec3(0.0, 1.0, 0.0));
  return r1 * r2 * viewing;
}

glm::mat4 Camera::perspective(float aspectRatio) {
  return glm::perspective(glm::radians(fovDegrees_), aspectRatio, near_, far_);
}

}  // namespace lve