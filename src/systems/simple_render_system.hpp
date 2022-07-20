#pragma once

#include "ecs/lve_ecs.hpp"
#include "lve_camera.hpp"
#include "lve_descriptors.hpp"
#include "lve_device.hpp"
#include "lve_frame_info.hpp"
#include "lve_game_object.hpp"
#include "lve_pipeline.hpp"
#include "lve_ubo.hpp"

// std
#include <memory>
#include <vector>

namespace lve {

struct TransformUboData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

class SimpleRenderSystem {
 public:
  SimpleRenderSystem(
      LveDevice &device,
      EntManager &ecs,
      VkRenderPass renderPass,
      VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;
  LveUbo<TransformUboData> transformUbo{lveDevice, 1000, false, true};

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;

  std::unique_ptr<LveDescriptorSetLayout> renderSystemLayout;

  EntQueryResult ents;
};
}  // namespace lve
