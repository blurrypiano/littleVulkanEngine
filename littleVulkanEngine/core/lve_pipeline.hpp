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

  static const ShaderLayout diffuse(std::string shaderDirPath) {
    return {
        shaderDirPath + "/diffuse.vert.spv",
        shaderDirPath + "/diffuse.frag.spv",
        {LveModel::VertexAttribute::POSITION,
         LveModel::VertexAttribute::NORMAL,
         LveModel::VertexAttribute::COLOR}};
  }

  static const ShaderLayout phong(std::string shaderDirPath) {
    return {
        shaderDirPath + "/phong.vert.spv",
        shaderDirPath + "/phong.frag.spv",
        {LveModel::VertexAttribute::POSITION,
         LveModel::VertexAttribute::NORMAL,
         LveModel::VertexAttribute::COLOR}};
  }

  static const ShaderLayout toon(std::string shaderDirPath) {
    return {
        shaderDirPath + "/toon.vert.spv",
        shaderDirPath + "/toon.frag.spv",
        {LveModel::VertexAttribute::POSITION,
         LveModel::VertexAttribute::NORMAL,
         LveModel::VertexAttribute::COLOR}};
  }
};

struct PipelineConfigInfo {
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  uint32_t subpass = 0;
};

class LvePipeline {
 public:
  LvePipeline(LveDevice& device, ShaderLayout shaderLayout, PipelineConfigInfo& configInfo);
  ~LvePipeline();
  void bind(VkCommandBuffer commandBuffer);

  static PipelineConfigInfo defaultFixedFunctionCreateInfo(LveSwapChain& swapChain_);

 private:
  static std::vector<char> readFile(const std::string& filename);

  void createGraphicsPipeline(PipelineConfigInfo& configInfo);
  VkShaderModule createShaderModule(const std::vector<char>& code);

  VkPipeline graphicsPipeline_;
  ShaderLayout shaderLayout_;
  LveDevice& device_;
};

}  // namespace lve
