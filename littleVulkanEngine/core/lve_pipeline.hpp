/*
 * Little Vulkan Engine Pipeline class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "lve_device.hpp"
#include "lve_swap_chain.hpp"

// libs
#include <vulkan/vulkan.h>

// std
#include <string>
#include <vector>

namespace lve {

class LvePipeline {
 public:
  LvePipeline(std::string filePrefix, LveDevice& device, LveSwapChain& swapChain);
  ~LvePipeline();

 private:
  static std::vector<char> readFile(const std::string& filename);

  void createGraphicsPipeline();
  VkShaderModule createShaderModule(const std::vector<char>& code);

  std::string filePrefix_;
  LveDevice& device_;
  LveSwapChain& swapChain_;

  VkPipelineLayout pipelineLayout_;
  VkPipeline graphicsPipeline_;
};

}  // namespace lve
