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

  LveWindow(int width, int height, std::string windowName) : mWidth{width}, mHeight{height} {
    mWindowName = windowName;
    initWindow();
  }

  ~LveWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  bool shouldClose() { return glfwWindowShouldClose(window); }
  bool wasWindowResized() { return framebufferResized; }
  void resetWindowResized() { framebufferResized = false; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
    }
  }

 private:
  void initWindow();
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

  GLFWwindow *window;
  const int mWidth;
  const int mHeight;
  std::string mWindowName;

  bool framebufferResized = false;
};

}  // namespace lve
