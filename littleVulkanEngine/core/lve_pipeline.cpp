/*
 * Little Vulkan Engine Pipeline class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_pipeline.hpp"

#include "lve_initializers.hpp"

// std
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace lve {

LvePipeline::LvePipeline(
    ShaderLayout shaderLayout,
    LveDevice& device,
    LveSwapChain& swapChain,
    VkPipelineLayout& pipelineLayout)
    : device_{device},
      shaderLayout_{shaderLayout},
      swapChain_{swapChain},
      pipelineLayout_{pipelineLayout} {
  createGraphicsPipeline();
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

void LvePipeline::createGraphicsPipeline() {
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

  auto inputAssembly = initializers::inputAssemblyState();
  VkExtent2D swapChainExtent = swapChain_.getSwapChainExtent();
  VkViewport viewport =
      {0.0f, 0.0f, (float)swapChainExtent.width, (float)swapChainExtent.height, 0.0f, 1.0f};
  VkRect2D scissor = {{0, 0}, swapChainExtent};
  auto viewportState = initializers::viewportState(viewport, scissor);
  auto rasterizer = initializers::rasterizationState();
  auto multisampling = initializers::multisampleState();
  auto colorBlendAttachment = initializers::colorBlendAttachmentState();
  auto colorBlending = initializers::colorBlendingState(colorBlendAttachment);
  auto depthStencil = initializers::depthStencilState();

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;

  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = nullptr;  // Optional
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = nullptr;  // Optional
  pipelineInfo.pDepthStencilState = &depthStencil;

  pipelineInfo.layout = pipelineLayout_;
  pipelineInfo.renderPass = swapChain_.getRenderPass();
  pipelineInfo.subpass = 0;

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

}  // namespace lve
