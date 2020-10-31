/*
 * Glfw window class
 *
 * Encapsulates a glfw window
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_window.hpp"

namespace lve {
void LveWindow::initWindow() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(mWidth, mHeight, mWindowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void LveWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto app = reinterpret_cast<LveWindow *>(glfwGetWindowUserPointer(window));
  app->framebufferResized = true;
}

}  // namespace lve
