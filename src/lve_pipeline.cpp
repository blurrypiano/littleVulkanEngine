#include "lve_pipeline.hpp"

#include "lve_model.hpp"

// std
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace lve {

LvePipeline::LvePipeline(
    LveDevice& device,
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipelineConfigInfo& configInfo)
    : lveDevice{device} {
  createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

LvePipeline::~LvePipeline() {
  lveDevice.device().destroyShaderModule(vertShaderModule);
  lveDevice.device().destroyShaderModule(fragShaderModule);
  lveDevice.device().destroyPipeline(graphicsPipeline);
}

std::vector<char> LvePipeline::readFile(const std::string& filepath) {
  std::string enginePath = ENGINE_DIR + filepath;
  std::ifstream file{enginePath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + enginePath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

void LvePipeline::createGraphicsPipeline(
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipelineConfigInfo& configInfo) {
  assert(
      configInfo.pipelineLayout != vk::PipelineLayout() &&
      "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
  assert(
      configInfo.renderPass != vk::RenderPass() &&
      "Cannot create graphics pipeline: no renderPass provided in configInfo");

  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  createShaderModule(vertCode, &vertShaderModule);
  createShaderModule(fragCode, &fragShaderModule);

  vk::PipelineShaderStageCreateInfo shaderStages[2];
  shaderStages[0].stage = vk::ShaderStageFlagBits::eVertex;
  shaderStages[0].module = vertShaderModule;
  shaderStages[0].pName = "main";
  // shaderStages [0].flags               = 0; // todo fix potencial error
  shaderStages[0].pNext = nullptr;
  shaderStages[0].pSpecializationInfo = nullptr;
  shaderStages[1].stage = vk::ShaderStageFlagBits::eFragment;
  shaderStages[1].module = fragShaderModule;
  shaderStages[1].pName = "main";
  // shaderStages [1].flags               = 0;// todo fix potencial error
  shaderStages[1].pNext = nullptr;
  shaderStages[1].pSpecializationInfo = nullptr;

  auto bindingDescriptions = LveModel::Vertex::getBindingDescriptions();
  auto attributeDescriptions = LveModel::Vertex::getAttributeDescriptions();
  vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
  vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

  vk::GraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
  pipelineInfo.pViewportState = &configInfo.viewportInfo;
  pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
  pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
  pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
  pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
  pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

  pipelineInfo.layout = configInfo.pipelineLayout;
  pipelineInfo.renderPass = configInfo.renderPass;
  pipelineInfo.subpass = configInfo.subpass;

  pipelineInfo.basePipelineIndex = -1;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  auto result = lveDevice.device().createGraphicsPipeline(nullptr, pipelineInfo);
  if (result.result != vk::Result::eSuccess) {
    throw std::runtime_error("failed to create graphics pipeline");
  }
  graphicsPipeline = result.value;
}

void LvePipeline::createShaderModule(
    const std::vector<char>& code, vk::ShaderModule* shaderModule) {
  vk::ShaderModuleCreateInfo createInfo{};
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
  *shaderModule = lveDevice.device().createShaderModule(createInfo);
}

void LvePipeline::bind(vk::CommandBuffer commandBuffer) {
  commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
}

void LvePipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo) {
  configInfo.inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
  configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  configInfo.viewportInfo.viewportCount = 1;
  configInfo.viewportInfo.pViewports = nullptr;
  configInfo.viewportInfo.scissorCount = 1;
  configInfo.viewportInfo.pScissors = nullptr;

  configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
  configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  configInfo.rasterizationInfo.polygonMode = vk::PolygonMode::eFill;
  configInfo.rasterizationInfo.lineWidth = 1.0f;
  configInfo.rasterizationInfo.cullMode = vk::CullModeFlagBits::eNone;
  configInfo.rasterizationInfo.frontFace = vk::FrontFace::eClockwise;
  configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
  configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
  configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
  configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

  configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
  configInfo.multisampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
  configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
  configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
  configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
  configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

  configInfo.colorBlendAttachment.colorWriteMask =
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
  configInfo.colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;   // Optional
  configInfo.colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero;  // Optional
  configInfo.colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;              // Optional
  configInfo.colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;   // Optional
  configInfo.colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;  // Optional
  configInfo.colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;              // Optional

  configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
  configInfo.colorBlendInfo.logicOp = vk::LogicOp::eCopy;  // Optional
  configInfo.colorBlendInfo.attachmentCount = 1;
  configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
  configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
  configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
  configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
  configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

  configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthCompareOp = vk::CompareOp::eLess;
  configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
  configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
  configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
  // configInfo.depthStencilInfo.front                 = { };  // Optional todo fix potencial error
  // configInfo.depthStencilInfo.back                  = { };  // Optional todo fix potencial error

  configInfo.dynamicStateEnables = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
  configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
  configInfo.dynamicStateInfo.dynamicStateCount =
      static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
  // configInfo.dynamicStateInfo.flags             = 0; //todo fix potencial error
}

}  // namespace lve
