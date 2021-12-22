#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace lve {
struct FrameInfo {
  int frameIndex;
  float frameTime;
  vk::CommandBuffer commandBuffer;
  LveCamera &camera;
  vk::DescriptorSet globalDescriptorSet;
  LveGameObject::Map &gameObjects;
};
}  // namespace lve
