/*
 * Little Vulkan Engine Pipeline class
 *
 * Copyright (C) 2020 by Brendan Galea - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_pipeline.hpp"

#include "lve_initializers.hpp"

// std
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace lve {

LvePipeline::LvePipeline(
    LveDevice& device, ShaderLayout shaderLayout, PipelineConfigInfo& configInfo)
    : device_{device}, shaderLayout_{shaderLayout} {
  createGraphicsPipeline(configInfo);
}

LvePipeline::~LvePipeline() { vkDestroyPipeline(device_.device(), graphicsPipeline_, nullptr); }

void LvePipeline::bind(VkCommandBuffer commandBuffer) {
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);
}

std::vector<char> LvePipeline::readFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + filename);
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

void LvePipeline::createGraphicsPipeline(PipelineConfigInfo& configInfo) {
  assert(configInfo.renderPass != nullptr && "Pipeline cannot be created with null render pass");
  assert(
      configInfo.pipelineLayout != nullptr &&
      "Pipeline cannot be created with null pipeline layout");

  // TODO abstract this away into shader layout
  auto vertCode = readFile(shaderLayout_.vertFilePath);
  auto fragCode = readFile(shaderLayout_.fragFilePath);

  VkShaderModule vertShaderModule = createShaderModule(vertCode);
  VkShaderModule fragShaderModule = createShaderModule(fragCode);
  auto vertShaderStageInfo =
      initializers::shaderStage(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
  auto fragShaderStageInfo =
      initializers::shaderStage(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);
  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  auto bindingDescription = LveModel::Vertex::getBindingDescription();
  auto attributeDescriptions =
      LveModel::Vertex::getAttributeDescriptions(shaderLayout_.vertexAttributes);
  auto vertexInputInfo = initializers::vertexInputState(bindingDescription, attributeDescriptions);

  auto viewportState = initializers::viewportState(configInfo.viewport, configInfo.scissor);

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;

  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
  pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
  pipelineInfo.pDepthStencilState = nullptr;  // Optional
  pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
  pipelineInfo.pDynamicState = nullptr;  // Optional
  pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;

  pipelineInfo.layout = configInfo.pipelineLayout;
  pipelineInfo.renderPass = configInfo.renderPass;
  pipelineInfo.subpass = configInfo.subpass;

  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;  // Optional
  pipelineInfo.basePipelineIndex = -1;               // Optional

  if (vkCreateGraphicsPipelines(
          device_.device(),
          VK_NULL_HANDLE,
          1,
          &pipelineInfo,
          nullptr,
          &graphicsPipeline_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline!");
  }

  vkDestroyShaderModule(device_.device(), fragShaderModule, nullptr);
  vkDestroyShaderModule(device_.device(), vertShaderModule, nullptr);
}

VkShaderModule LvePipeline::createShaderModule(const std::vector<char>& code) {
  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device_.device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module!");
  }
  return shaderModule;
}

/**
 * Helper function to create default fixed function create info using swap chain
 *
 * @param swapChain Active swapchain
 *
 * @return FixedFunctionCreateInfo for default pipeline, pipeline layout needs to be set
 */
PipelineConfigInfo LvePipeline::defaultFixedFunctionCreateInfo(LveSwapChain& swapChain) {
  VkExtent2D swapChainExtent = swapChain.getSwapChainExtent();
  PipelineConfigInfo configInfo{};
  configInfo.inputAssemblyInfo = initializers::inputAssemblyState();
  configInfo.viewport =
      {0.0f, 0.0f, (float)swapChainExtent.width, (float)swapChainExtent.height, 0.0f, 1.0f};
  configInfo.scissor = {{0, 0}, swapChainExtent};
  configInfo.rasterizationInfo = initializers::rasterizationState();
  configInfo.multisampleInfo = initializers::multisampleState();
  configInfo.colorBlendAttachment = initializers::colorBlendAttachmentState();
  configInfo.colorBlendInfo = initializers::colorBlendingState(configInfo.colorBlendAttachment);
  configInfo.depthStencilInfo = initializers::depthStencilState();
  configInfo.subpass = 0;
  configInfo.renderPass = swapChain.getRenderPass();
  return configInfo;
}
}  // namespace lve
