#pragma once

#include "rendering/lve_camera.hpp"
#include "rendering/lve_device.hpp"
#include "rendering/lve_frame_info.hpp"
#include "rendering/lve_game_object.hpp"
#include "rendering/lve_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class PointLightSystem {
 public:
  PointLightSystem(
      LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void renderLights(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace lve
