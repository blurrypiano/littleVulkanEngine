#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

struct SimplePushConstantData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(
    LveDevice& device,
    EntManager& ecs,
    VkRenderPass renderPass,
    VkDescriptorSetLayout globalSetLayout)
    : lveDevice{device}, ents{ecs.allOf<TransformComponent, ModelComponent>()} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
  vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  renderSystemLayout = LveDescriptorSetLayout::Builder(lveDevice)
                           .addBinding(
                               0,
                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
                           .build();

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts{
      globalSetLayout,
      renderSystemLayout->getDescriptorSetLayout()};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
  pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  lvePipeline = std::make_unique<LvePipeline>(
      lveDevice,
      "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv",
      pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo) {
  lvePipeline->bind(frameInfo.commandBuffer);

  vkCmdBindDescriptorSets(
      frameInfo.commandBuffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      pipelineLayout,
      0,
      1,
      &frameInfo.globalDescriptorSet,
      0,
      nullptr);

  // transform ubo system could create a T iterator wrapper
  // update transformUbo for each game object
  // LveUboRegion

  // vulkan memory allocator
  // avoid buffer allocation limit
  // easier to grow

  int index = 0;
  for (auto [transform, entId] : ents.iterate<TransformComponent>()) {
    TransformUboData& transformData = transformUbo.get(frameInfo.frameIndex, index);
    transformData.modelMatrix = transform.mat4();
    transformData.normalMatrix = transform.normalMatrix();
    index += 1;
  }
  transformUbo.flushRegion(frameInfo.frameIndex);

  // render each game object
  index = 0;
  for (auto [transform, model, entId] : ents.iterate<TransformComponent, ModelComponent>()) {
    auto bufferInfo = transformUbo.bufferInfoForElement(frameInfo.frameIndex, index);
    VkDescriptorSet transformDescriptorSet;
    LveDescriptorWriter(*renderSystemLayout, frameInfo.frameDescriptorPool)
        .writeBuffer(0, &bufferInfo)
        .build(transformDescriptorSet);

    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        1,  // starting set (0 is the globalDescriptorSet, 1 is the set specific to this system)
        1,  // binding 1 more set
        &transformDescriptorSet,
        0,
        nullptr);

    SimplePushConstantData push{};
    push.modelMatrix = transform.mat4();
    push.normalMatrix = transform.normalMatrix();

    vkCmdPushConstants(
        frameInfo.commandBuffer,
        pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(SimplePushConstantData),
        &push);
    model.model->bind(frameInfo.commandBuffer);
    model.model->draw(frameInfo.commandBuffer);

    index += 1;
  }
}

}  // namespace lve
