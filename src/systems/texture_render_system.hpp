#pragma once

#include "lve_camera.hpp"
#include "lve_descriptors.hpp"
#include "lve_device.hpp"
#include "lve_frame_info.hpp"
#include "lve_game_object.hpp"
#include "lve_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class TextureRenderSystem {
 public:
  TextureRenderSystem(
      LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~TextureRenderSystem();

  TextureRenderSystem(const TextureRenderSystem &) = delete;
  TextureRenderSystem &operator=(const TextureRenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;

  std::unique_ptr<LveDescriptorSetLayout> renderSystemLayout;
};
}  // namespace lve
