#pragma once

#include "lve_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace lve {

class LveDescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(LveDevice &lveDevice) : lveDevice{lveDevice} {}

    Builder &addBinding(
        uint32_t binding,
        vk::DescriptorType descriptorType,
        vk::ShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<LveDescriptorSetLayout> build() const;

   private:
    LveDevice &lveDevice;
    std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings{};
  };

  LveDescriptorSetLayout(
      LveDevice &lveDevice, std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings);
  ~LveDescriptorSetLayout();
  LveDescriptorSetLayout(const LveDescriptorSetLayout &) = delete;
  LveDescriptorSetLayout &operator=(const LveDescriptorSetLayout &) = delete;

  vk::DescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  LveDevice &lveDevice;
  vk::DescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings;

  friend class LveDescriptorWriter;
};

class LveDescriptorPool {
 public:
  class Builder {
   public:
    Builder(LveDevice &lveDevice) : lveDevice{lveDevice} {}

    Builder &addPoolSize(vk::DescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(vk::DescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<LveDescriptorPool> build() const;

   private:
    LveDevice &lveDevice;
    std::vector<vk::DescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    vk::DescriptorPoolCreateFlags poolFlags;  // todo fix potencial error  =0
  };

  LveDescriptorPool(
      LveDevice &lveDevice,
      uint32_t maxSets,
      vk::DescriptorPoolCreateFlags poolFlags,
      const std::vector<vk::DescriptorPoolSize> &poolSizes);
  ~LveDescriptorPool();
  LveDescriptorPool(const LveDescriptorPool &) = delete;
  LveDescriptorPool &operator=(const LveDescriptorPool &) = delete;

  bool allocateDescriptor(
      const vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<vk::DescriptorSet> &descriptors) const;

  void resetPool();

 private:
  LveDevice &lveDevice;
  vk::DescriptorPool descriptorPool;

  friend class LveDescriptorWriter;
};

class LveDescriptorWriter {
 public:
  LveDescriptorWriter(LveDescriptorSetLayout &setLayout, LveDescriptorPool &pool);

  LveDescriptorWriter &writeBuffer(uint32_t binding, vk::DescriptorBufferInfo *bufferInfo);
  LveDescriptorWriter &writeImage(uint32_t binding, vk::DescriptorImageInfo *imageInfo);

  bool build(vk::DescriptorSet &set);
  void overwrite(vk::DescriptorSet &set);

 private:
  LveDescriptorSetLayout &setLayout;
  LveDescriptorPool &pool;
  std::vector<vk::WriteDescriptorSet> writes;
};

}  // namespace lve
