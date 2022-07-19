#pragma once

#include "lve_buffer.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_utils.hpp"

// std
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

namespace lve {

template <typename T>
class LveUbo {
 public:
  LveUbo(
      LveDevice &device,
      int instancesPerRegion,
      bool flushablePerElement = true,
      bool descriptorInfoPerElement = true,
      int numRegions = LveSwapChain::MAX_FRAMES_IN_FLIGHT)
      : lveDevice{device},
        instancesPerRegion{instancesPerRegion},
        instanceSize{sizeof(T)},
        numRegions{numRegions},
        flushablePerElement{flushablePerElement},
        descriptorInfoPerElement{descriptorInfoPerElement} {
    calculateAlignmentAndRegionSize();

    buffer = std::make_unique<LveBuffer>(
        device,
        regionSize * numRegions,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    buffer->map();
    printInfo();
  }

  void printInfo() {
    const auto nonCoherentAtomSize = lveDevice.properties.limits.nonCoherentAtomSize;
    const auto minUniformBufferOffsetAlignment =
        lveDevice.properties.limits.minUniformBufferOffsetAlignment;
    std::cout << "InstanceSize: " << sizeof(T) << std::endl;
    std::cout << "AlignmentPerInstance: " << alignmentPerInstance << std::endl;
    std::cout << "instancesPerRegion: " << instancesPerRegion << std::endl;
    std::cout << "numRegions: " << numRegions << std::endl;
    std::cout << "regionSize: " << regionSize << std::endl;
    std::cout << "nonCoherentAtomSize: " << nonCoherentAtomSize << std::endl;
    std::cout << "minUniformBufferOffsetAlignment: " << minUniformBufferOffsetAlignment
              << std::endl;
  }

  // TODO constructor for non-dynamic buffer
  // takes all data at initialization, stores in device only memory

  LveUbo(const LveUbo &) = delete;
  LveUbo &operator=(const LveUbo &) = delete;
  LveUbo(LveUbo &&) = delete;
  LveUbo &operator=(LveUbo &&) = delete;

  T &get(int frameIndex, int elementIndex = 0) {
    assert(frameIndex < numRegions && "Trying to write to region outside ubo range");
    assert(elementIndex < instancesPerRegion && "Trying to write to instance outside ubo range");

    char *mapped = static_cast<char *>(buffer->getMappedMemory());
    assert(mapped != nullptr && "Cannot get element if buffer is not mapped");

    int offset = frameIndex * regionSize + elementIndex * alignmentPerInstance;
    return (*static_cast<T *>(reinterpret_cast<void *>(mapped + offset)));
  }

  void write(T &item, int frameIndex, int elementIndex = 0) {
    assert(frameIndex < numRegions && "Trying to write to region outside ubo range");
    assert(elementIndex < instancesPerRegion && "Trying to write to instance outside ubo range");
    buffer->writeToBuffer(
        (void *)&item,
        instanceSize,
        frameIndex * regionSize + elementIndex * alignmentPerInstance);
  }

  void flushRegion(int frameIndex) {
    assert(frameIndex < numRegions && "Trying to flush to region outside ubo range");
    buffer->flush(regionSize, regionSize * frameIndex);
  }

  void flushRange(int frameIndex, int elementStart, int elementEnd) {
    assert(frameIndex < numRegions && "Trying to flush element in region outside ubo range");
    assert(elementStart < instancesPerRegion && "Trying to flush element outside ubo range");
    assert(elementEnd < instancesPerRegion && "Trying to flush element outside ubo range");
    assert(elementStart < elementEnd && "Must have start < end to flush range");
    assert(
        flushablePerElement &&
        "Cannot call flushRange if not initialized with flushablePerElement=true");
    buffer->flush(
        alignmentPerInstance * (elementEnd - elementStart),
        frameIndex * regionSize + alignmentPerInstance * elementStart);
  }

  void flushElement(int frameIndex, int elementIndex) {
    assert(frameIndex < numRegions && "Trying to flush element in region outside ubo range");
    assert(elementIndex < instancesPerRegion && "Trying to flush element outside ubo range");
    assert(
        flushablePerElement &&
        "Cannot call flushElement if not initialized with flushablePerElement=true");
    buffer->flush(
        alignmentPerInstance,
        frameIndex * regionSize + alignmentPerInstance * elementIndex);
  }

  void invalidate(int frameIndex);
  void invalidate(int frameIndex, int elementIndex);

  VkDescriptorBufferInfo bufferInfoForRegion(int frameIndex) const {
    assert(frameIndex < numRegions && "Trying to get descriptorInfo for region outside ubo range");
    return buffer->descriptorInfo(regionSize, regionSize * frameIndex);
  }

  VkDescriptorBufferInfo bufferInfoForElement(int frameIndex, int elementIndex) const {
    assert(frameIndex < numRegions && "Trying to flush element in region outside ubo range");
    assert(elementIndex < instancesPerRegion && "Trying to flush element outside ubo range");
    assert(
        descriptorInfoPerElement &&
        "Cannot call descriptorInfoForElement if not initialized with "
        "descriptorInfoPerElement=true");
    return buffer->descriptorInfo(
        alignmentPerInstance,
        frameIndex * regionSize + elementIndex * alignmentPerInstance);
  }

  // can make thos stack allocated potentially
  std::unique_ptr<LveBuffer> buffer;

 private:
  LveDevice &lveDevice;
  int numRegions;
  int regionSize;
  int instancesPerRegion;
  int alignmentPerInstance;
  int instanceSize;
  bool flushablePerElement;
  bool descriptorInfoPerElement;

  VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
    if (minOffsetAlignment > 0) {
      return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    }
    return instanceSize;
  }

  void calculateAlignmentAndRegionSize() {
    const auto nonCoherentAtomSize = lveDevice.properties.limits.nonCoherentAtomSize;
    const auto minUniformBufferOffsetAlignment =
        lveDevice.properties.limits.minUniformBufferOffsetAlignment;

    // calculate alignmentPerInstance
    if (flushablePerElement && descriptorInfoPerElement) {
      alignmentPerInstance = getAlignment(
          instanceSize,
          std::lcm(nonCoherentAtomSize, minUniformBufferOffsetAlignment));
    } else if (flushablePerElement && !descriptorInfoPerElement) {
      alignmentPerInstance = getAlignment(instanceSize, nonCoherentAtomSize);
    } else if (!flushablePerElement && descriptorInfoPerElement) {
      alignmentPerInstance = getAlignment(instanceSize, minUniformBufferOffsetAlignment);
    } else {
      alignmentPerInstance = instanceSize;
    }

    // regions must always be flushable and provide buffer info
    regionSize = instancesPerRegion * alignmentPerInstance;
    regionSize =
        getAlignment(regionSize, std::lcm(nonCoherentAtomSize, minUniformBufferOffsetAlignment));
  }
};

}  // namespace lve