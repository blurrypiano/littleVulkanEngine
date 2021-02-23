/*
 * Encapsulates a vulkan buffer
 *
 * Copyright (C) 2020 by Brendan Galea - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

// proj
#include "lve_device.hpp"

// libs
#include <vulkan/vulkan.h>

// std
#include <memory>
#include <string>

namespace lve {

class LveBuffer {
 public:
  struct Props {
    VkDeviceSize size;
    VkDeviceSize alignment;
    VkDeviceSize instanceSize;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;
    int instanceCount;
  };

  LveBuffer(
      LveDevice& device,
      int instanceSize,
      int instanceCount,
      VkBufferUsageFlags usageFlags,
      VkMemoryPropertyFlags memoryPropertyFlags);
  ~LveBuffer();

  LveBuffer(const LveBuffer&) = delete;
  LveBuffer& operator=(const LveBuffer&) = delete;

  void create();
  VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  void unmap();
  VkResult bind(VkDeviceSize offset = 0);
  void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  void copyTo(void* data, VkDeviceSize size);
  void copyIndexed(void* data, int index);
  VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  VkResult flushIndexed(int index);
  VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  VkDescriptorBufferInfo descriptorIndexed(int index);
  void destroy();

  VkBuffer buffer = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  VkDescriptorBufferInfo descriptor;
  void* mapped = nullptr;
  Props props;

 private:
  LveDevice& device_;
};

}  // namespace lve