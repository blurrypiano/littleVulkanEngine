/*
 * LveApp class
 *
 * Starting point for a little vulkan engine application
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_app.hpp"

namespace lve {

void LveApp::run() {
  while (!window_.shouldClose()) {
    glfwPollEvents();
  }
}

}  // namespace lve
