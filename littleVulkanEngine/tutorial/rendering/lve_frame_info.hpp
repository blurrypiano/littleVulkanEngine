#pragma once

#include "rendering/lve_camera.hpp"
#include "rendering/lve_descriptors.hpp"
#include "rendering/lve_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace lve {
struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  LveCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  LveGameObject::Map &gameObjects;
};
}  // namespace lve
