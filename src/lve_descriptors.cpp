#include "lve_descriptors.hpp"

// std
#include <cassert>
#include <stdexcept>

namespace lve {

// *************** Descriptor Set Layout Builder *********************

LveDescriptorSetLayout::Builder &LveDescriptorSetLayout::Builder::addBinding(
    uint32_t binding,
    vk::DescriptorType descriptorType,
    vk::ShaderStageFlags stageFlags,
    uint32_t count) {
  assert(bindings.count(binding) == 0 && "Binding already in use");
  vk::DescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = binding;
  layoutBinding.descriptorType = descriptorType;
  layoutBinding.descriptorCount = count;
  layoutBinding.stageFlags = stageFlags;
  bindings[binding] = layoutBinding;
  return *this;
}

std::unique_ptr<LveDescriptorSetLayout> LveDescriptorSetLayout::Builder::build() const {
  return std::make_unique<LveDescriptorSetLayout>(lveDevice, bindings);
}

// *************** Descriptor Set Layout *********************

LveDescriptorSetLayout::LveDescriptorSetLayout(
    LveDevice &lveDevice, std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings)
    : lveDevice{lveDevice}, bindings{bindings} {
  std::vector<vk::DescriptorSetLayoutBinding> setLayoutBindings{};
  for (auto kv : bindings) {
    setLayoutBindings.push_back(kv.second);
  }

  vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
  descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();
  descriptorSetLayout = lveDevice.device().createDescriptorSetLayout(descriptorSetLayoutInfo);
}

LveDescriptorSetLayout::~LveDescriptorSetLayout() {
  lveDevice.device().destroyDescriptorSetLayout(descriptorSetLayout);
}

// *************** Descriptor Pool Builder *********************

LveDescriptorPool::Builder &LveDescriptorPool::Builder::addPoolSize(
    vk::DescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
  return *this;
}

LveDescriptorPool::Builder &LveDescriptorPool::Builder::setPoolFlags(
    vk::DescriptorPoolCreateFlags flags) {
  poolFlags = flags;
  return *this;
}
LveDescriptorPool::Builder &LveDescriptorPool::Builder::setMaxSets(uint32_t count) {
  maxSets = count;
  return *this;
}

std::unique_ptr<LveDescriptorPool> LveDescriptorPool::Builder::build() const {
  return std::make_unique<LveDescriptorPool>(lveDevice, maxSets, poolFlags, poolSizes);
}

// *************** Descriptor Pool *********************

LveDescriptorPool::LveDescriptorPool(
    LveDevice &lveDevice,
    uint32_t maxSets,
    vk::DescriptorPoolCreateFlags poolFlags,
    const std::vector<vk::DescriptorPoolSize> &poolSizes)
    : lveDevice{lveDevice} {
  vk::DescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  descriptorPoolInfo.pPoolSizes = poolSizes.data();
  descriptorPoolInfo.maxSets = maxSets;
  descriptorPoolInfo.flags = poolFlags;
  descriptorPool = lveDevice.device().createDescriptorPool(descriptorPoolInfo);
}

LveDescriptorPool::~LveDescriptorPool() {
  lveDevice.device().destroyDescriptorPool(descriptorPool);
}

bool LveDescriptorPool::allocateDescriptor(
    const vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorSet &descriptor) const {
  vk::DescriptorSetAllocateInfo allocInfo{};
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.pSetLayouts = &descriptorSetLayout;
  allocInfo.descriptorSetCount = 1;

  // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
  // a new pool whenever an old pool fills up. But this is beyond our current scope
  descriptor = lveDevice.device().allocateDescriptorSets(allocInfo)[0];
  return true;
}

void LveDescriptorPool::freeDescriptors(std::vector<vk::DescriptorSet> &descriptors) const {
  lveDevice.device().freeDescriptorSets(descriptorPool, descriptors);
}

void LveDescriptorPool::resetPool() { lveDevice.device().resetDescriptorPool(descriptorPool); }

// *************** Descriptor Writer *********************

LveDescriptorWriter::LveDescriptorWriter(LveDescriptorSetLayout &setLayout, LveDescriptorPool &pool)
    : setLayout{setLayout}, pool{pool} {}

LveDescriptorWriter &LveDescriptorWriter::writeBuffer(
    uint32_t binding, vk::DescriptorBufferInfo *bufferInfo) {
  assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

  auto &bindingDescription = setLayout.bindings[binding];

  assert(
      bindingDescription.descriptorCount == 1 &&
      "Binding single descriptor info, but binding expects multiple");

  vk::WriteDescriptorSet write{};
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pBufferInfo = bufferInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

LveDescriptorWriter &LveDescriptorWriter::writeImage(
    uint32_t binding, vk::DescriptorImageInfo *imageInfo) {
  assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

  auto &bindingDescription = setLayout.bindings[binding];

  assert(
      bindingDescription.descriptorCount == 1 &&
      "Binding single descriptor info, but binding expects multiple");

  vk::WriteDescriptorSet write{};
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pImageInfo = imageInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

bool LveDescriptorWriter::build(vk::DescriptorSet &set) {
  bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
  if (!success) {
    return false;
  }
  overwrite(set);
  return true;
}

void LveDescriptorWriter::overwrite(vk::DescriptorSet &set) {
  for (auto &write : writes) {
    write.dstSet = set;
  }
  pool.lveDevice.device().updateDescriptorSets(writes, nullptr);
}

}  // namespace lve
