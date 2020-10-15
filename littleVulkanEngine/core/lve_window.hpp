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
#include <string>
#include <vector>

namespace lve {

class LveWindow {
 private:
  const uint32_t mWidth;
  const uint32_t mHeight;

  std::string mWindowName;

  bool framebufferResized = false;

 public:
  GLFWwindow *window;

  LveWindow(int width, int height) : LveWindow(width, height, "Vulkan Application") {}

  LveWindow(int width, int height, std::string windowName)
      : mWidth{static_cast<uint32_t>(width)}, mHeight{static_cast<uint32_t>(height)} {
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

 private:
  void initWindow();
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
};

}  // namespace lve
