/*
 * Little Vulkan Engine Pipeline class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "lve_device.hpp"
#include "lve_model.hpp"
#include "lve_swap_chain.hpp"

// libs
#include <vulkan/vulkan.h>

// std
#include <string>
#include <vector>

namespace lve {

struct ShaderLayout {
  const std::string vertFilePath;
  const std::string fragFilePath;
  const std::vector<LveModel::VertexAttribute> vertexAttributes;

  static const ShaderLayout simple(std::string shaderDirPath) {
    return {
        shaderDirPath + "/simple.vert.spv",
        shaderDirPath + "/simple.frag.spv",
        {LveModel::VertexAttribute::POSITION}};
  }
};

class LvePipeline {
 public:
  LvePipeline(ShaderLayout shaderLayout, LveDevice& device, LveSwapChain& swapChain);
  ~LvePipeline();
  void bind(VkCommandBuffer commandBuffer);

 private:
  static std::vector<char> readFile(const std::string& filename);

  void createGraphicsPipeline();
  VkShaderModule createShaderModule(const std::vector<char>& code);

  ShaderLayout shaderLayout_;
  LveDevice& device_;
  LveSwapChain& swapChain_;

  VkPipelineLayout pipelineLayout_;
  VkPipeline graphicsPipeline_;
};

}  // namespace lve
