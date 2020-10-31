/*
 * Glfw window class
 *
 * Encapsulates a glfw window
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std lib headers
#include <stdexcept>
#include <string>
#include <vector>

namespace lve {

class LveWindow {
 public:
  LveWindow(int width, int height) : LveWindow(width, height, "Vulkan Application") {}

  LveWindow(int width, int height, std::string windowName) : width_{width}, height_{height} {
    windowName_ = windowName;
    initWindow();
  }

  ~LveWindow() {
    glfwDestroyWindow(window_);
    glfwTerminate();
  }

  bool shouldClose() { return glfwWindowShouldClose(window_); }
  bool wasWindowResized() { return framebufferResized_; }
  void resetWindowResized() { framebufferResized_ = false; }

  int height() { return height_; }
  int width() { return width_; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window_, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
    }
  }

 private:
  void initWindow();
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

  GLFWwindow *window_;
  int width_;
  int height_;
  std::string windowName_;

  bool framebufferResized_ = false;
};

}  // namespace lve
