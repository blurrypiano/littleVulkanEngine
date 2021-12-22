#pragma once

#include "lve_device.hpp"

namespace lve {

class LveBuffer {
 public:
  LveBuffer(
      LveDevice& device,
      vk::DeviceSize instanceSize,
      uint32_t instanceCount,
      vk::BufferUsageFlags usageFlags,
      vk::MemoryPropertyFlags memoryPropertyFlags,
      vk::DeviceSize minOffsetAlignment = 1);
  ~LveBuffer();

  LveBuffer(const LveBuffer&) = delete;
  LveBuffer& operator=(const LveBuffer&) = delete;

  vk::Result map(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
  void unmap();

  void writeToBuffer(void* data, vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
  vk::Result flush(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
  vk::DescriptorBufferInfo descriptorInfo(
      vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
  vk::Result invalidate(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);

  void writeToIndex(void* data, int index);
  vk::Result flushIndex(int index);
  vk::DescriptorBufferInfo descriptorInfoForIndex(int index);
  vk::Result invalidateIndex(int index);

  vk::Buffer getBuffer() const { return buffer; }
  void* getMappedMemory() const { return mapped; }
  uint32_t getInstanceCount() const { return instanceCount; }
  vk::DeviceSize getInstanceSize() const { return instanceSize; }
  vk::DeviceSize getAlignmentSize() const { return instanceSize; }
  vk::BufferUsageFlags getUsageFlags() const { return usageFlags; }
  vk::MemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }
  vk::DeviceSize getBufferSize() const { return bufferSize; }

 private:
  static vk::DeviceSize getAlignment(
      vk::DeviceSize instanceSize, vk::DeviceSize minOffsetAlignment);

  LveDevice& lveDevice;
  void* mapped = nullptr;
  vk::Buffer buffer = VK_NULL_HANDLE;
  vk::DeviceMemory memory = VK_NULL_HANDLE;

  vk::DeviceSize bufferSize;
  uint32_t instanceCount;
  vk::DeviceSize instanceSize;
  vk::DeviceSize alignmentSize;
  vk::BufferUsageFlags usageFlags;
  vk::MemoryPropertyFlags memoryPropertyFlags;
};

}  // namespace lve
