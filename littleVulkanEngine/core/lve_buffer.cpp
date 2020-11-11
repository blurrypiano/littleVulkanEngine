/*
 * Encapsulates a vulkan buffer
 *
 * Based off VulkanBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_buffer.hpp"

#include <cassert>
#include <cstring>

namespace lve {

LveBuffer::LveBuffer(
    LveDevice &device,
    int instanceSize,
    int instanceCount,
    VkBufferUsageFlags usageFlags,
    VkMemoryPropertyFlags memoryPropertyFlags)
    : device_{device} {
  size_t minUboAlignment = device_.properties.limits.minUniformBufferOffsetAlignment;
  props.alignment = instanceSize;
  if (minUboAlignment > 0) {
    props.alignment = (props.alignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
  }
  props.instanceSize = instanceSize;
  props.size = instanceCount * props.alignment;
  props.memoryPropertyFlags = memoryPropertyFlags;
  props.usageFlags = usageFlags;
  props.instanceCount = instanceCount;
}

LveBuffer::~LveBuffer() { destroy(); }

/**
 * Obtains all Vulkan resources held by this buffer
 */
void LveBuffer::create() {
  assert(!buffer && !memory && "Invalid to call create on LveBuffer twice");
  device_.createBuffer(props.size, props.usageFlags, props.memoryPropertyFlags, buffer, memory);
  setupDescriptor();
}

/**
 * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
 *
 * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
 * buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the buffer mapping call
 */
VkResult LveBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
  // TODO always map whole range?
  assert(buffer && memory && "Called map on buffer before create");
  if (size == VK_WHOLE_SIZE) {
    return vkMapMemory(device_.device(), memory, 0, props.size, 0, &mapped);
  }
  return vkMapMemory(device_.device(), memory, offset, size, 0, &mapped);
}

/**
 * Unmap a mapped memory range
 *
 * @note Does not return a result as vkUnmapMemory can't fail
 */
void LveBuffer::unmap() {
  if (mapped) {
    vkUnmapMemory(device_.device(), memory);
    mapped = nullptr;
  }
}

/**
 * Attach the allocated memory block to the buffer
 *
 * @param offset (Optional) Byte offset (from the beginning) for the memory region to bind
 *
 * @return VkResult of the bindBufferMemory call
 */
VkResult LveBuffer::bind(VkDeviceSize offset) {
  // TODO remove? already binding in create buffer
  return vkBindBufferMemory(device_.device(), buffer, memory, offset);
}

/**
 * Setup the default descriptor for this buffer
 *
 * @param size (Optional) Size of the memory range of the descriptor
 * @param offset (Optional) Byte offset from beginning
 *
 */
void LveBuffer::setupDescriptor(VkDeviceSize size, VkDeviceSize offset) {
  descriptor.offset = offset;
  descriptor.buffer = buffer;
  descriptor.range = size;
}

/**
 * @param index The index of the desired memory offset
 *
 * @return VkDescriptorBufferInfo corresponding to index
 */
VkDescriptorBufferInfo LveBuffer::descriptorIndexed(int index) {
  assert(index >= 0 && index < props.instanceCount);
  VkDescriptorBufferInfo bufferInfo{};
  bufferInfo.offset = index * props.alignment;
  bufferInfo.buffer = buffer;
  bufferInfo.range = props.instanceSize;
  return bufferInfo;
}

/**
 * Copies the specified data to the mapped buffer
 *
 * @param data Pointer to the data to copy
 * @param size Size of the data to copy in machine units
 *
 */
void LveBuffer::copyTo(void *data, VkDeviceSize size) {
  assert(mapped && "Cannot copy to unmapped buffer");
  memcpy(mapped, data, size);
}

/**
 * Copies the specified data to the mapped buffer at the correct offset specified by index,
 * accounting for device alignment
 *
 * @param data Pointer to the data to copy
 * @param index The index of the memory range to map, uses an offset of index * alignment, and maps
 * size of buffer range.
 *
 */
void LveBuffer::copyIndexed(void *data, int index) {
  assert(mapped && "Cannot copy to unmapped buffer");
  char *memOffset = (char *)mapped;
  memOffset += props.alignment * index;
  assert(
      props.alignment * index + props.instanceSize < props.size &&
      "Trying to write outside of buffer!");
  memcpy((void *)memOffset, data, props.instanceSize);
}

/**
 * Flush a memory range of the buffer to make it visible to the device_.device()
 *
 * @note Only required for non-coherent memory
 *
 * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
 * complete buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the flush call
 */
VkResult LveBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
  VkMappedMemoryRange mappedRange = {};
  mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mappedRange.memory = memory;
  mappedRange.offset = offset;
  mappedRange.size = size;
  return vkFlushMappedMemoryRanges(device_.device(), 1, &mappedRange);
}

/**
 * Flush a memory range of the buffer to make it visible to the device for specified index
 *
 * @note Only required for non-coherent memory
 *
 * @param index Index to flush buffer range.
 *
 * @return VkResult of the flush call
 */
VkResult LveBuffer::flushIndexed(int index) {
  return flush(props.alignment, index * props.alignment);
}

/**
 * Invalidate a memory range of the buffer to make it visible to the host
 *
 * @note Only required for non-coherent memory
 *
 * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
 * the complete buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the invalidate call
 */
VkResult LveBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
  VkMappedMemoryRange mappedRange = {};
  mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mappedRange.memory = memory;
  mappedRange.offset = offset;
  mappedRange.size = size;
  return vkInvalidateMappedMemoryRanges(device_.device(), 1, &mappedRange);
}

/**
 * Release all Vulkan resources held by this buffer
 */
void LveBuffer::destroy() {
  if (buffer) {
    vkDestroyBuffer(device_.device(), buffer, nullptr);
  }
  if (memory) {
    vkFreeMemory(device_.device(), memory, nullptr);
  }
  buffer = VK_NULL_HANDLE;
  memory = VK_NULL_HANDLE;
}
}  // namespace lve