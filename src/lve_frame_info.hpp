#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"

// lib
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

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
