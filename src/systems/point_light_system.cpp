#include "point_light_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <map>
#include <stdexcept>

namespace lve {

struct PointLightPushConstants {
  glm::vec4 position{};
  glm::vec4 color{};
  float radius;
};

PointLightSystem::PointLightSystem(
    LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    : lveDevice{device} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
  vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(PointLightPushConstants);

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

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

void PointLightSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
  LvePipeline::enableAlphaBlending(pipelineConfig);
  pipelineConfig.attributeDescriptions.clear();
  pipelineConfig.bindingDescriptions.clear();
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  lvePipeline = std::make_unique<LvePipeline>(
      lveDevice,
      "shaders/point_light.vert.spv",
      "shaders/point_light.frag.spv",
      pipelineConfig);
}

void PointLightSystem::update(FrameInfo& frameInfo, GlobalUbo& ubo) {
  auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.frameTime, {0.f, -1.f, 0.f});
  int lightIndex = 0;
  // TODO store pointLightQueryResult
  auto pointLights = frameInfo.ecs.allOf<PointLightComponent>();
  for (auto& light : pointLights) {
    assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

    auto& lightTransform = light.get<TransformComponent>();

    // update light position
    lightTransform.translation =
        glm::vec3(rotateLight * glm::vec4(lightTransform.translation, 1.f));

    // copy light to ubo
    ubo.pointLights[lightIndex].position = glm::vec4(lightTransform.translation, 1.f);
    ubo.pointLights[lightIndex].color = glm::vec4(
        light.get<ColorComponent>().color,
        light.get<PointLightComponent>().lightIntensity);

    lightIndex += 1;
  }
  ubo.numLights = lightIndex;
}

void PointLightSystem::render(FrameInfo& frameInfo) {
  // sort lights
  std::map<float, EntId> sorted;
  auto pointLights = frameInfo.ecs.allOf<PointLightComponent>();
  for (auto lightId : pointLights.ids()) {
    auto& lightTransform = frameInfo.ecs.get<TransformComponent>(lightId);

    // calculate distance
    auto offset = frameInfo.camera.getPosition() - lightTransform.translation;
    float disSquared = glm::dot(offset, offset);
    sorted[disSquared] = lightId;
  }

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

  // iterate through sorted lights in reverse order
  for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
    // use game obj id to find light object
    auto lightId = it->second;
    auto& lightTransform = frameInfo.ecs.get<TransformComponent>(lightId);
    auto& lightColor = frameInfo.ecs.get<ColorComponent>(lightId);
    auto& lightComp = frameInfo.ecs.get<PointLightComponent>(lightId);

    PointLightPushConstants push{};
    push.position = glm::vec4(lightTransform.translation, 1.f);
    push.color = glm::vec4(lightColor.color, lightComp.lightIntensity);
    push.radius = lightTransform.scale.x;

    vkCmdPushConstants(
        frameInfo.commandBuffer,
        pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(PointLightPushConstants),
        &push);
    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
  }
}

}  // namespace lve
