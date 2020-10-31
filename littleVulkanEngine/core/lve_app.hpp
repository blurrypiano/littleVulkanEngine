/*
 * LveApp class
 *
 * Starting point for a little vulkan engine application
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "lve_device.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

// std
#include <vector>

namespace lve {

class LveApp {
 public:
  LveApp();
  ~LveApp();

  void run();

  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

 private:
  void createCommandBuffers();
  void drawFrame();

  LveWindow window_{WIDTH, HEIGHT, "Hello Vulkan!"};
  LveDevice device_{window_};
  LveSwapChain swapChain_{window_, device_};
  LvePipeline pipeline_{"littleVulkanEngine/core/simple_shader", device_, swapChain_};

  std::vector<VkCommandBuffer> commandBuffers_;
};

}  // namespace lve
