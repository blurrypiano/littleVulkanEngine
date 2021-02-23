/*
 * Model Texture class
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
class LveTexture {
 private:
  struct _constructor_tag {
    explicit _constructor_tag() = default;
  };

 public:
  LveTexture(_constructor_tag, LveDevice &device) : device_{device} {}

  ~LveTexture() {
    vkDestroySampler(device_.device(), textureSampler_, nullptr);
    vkDestroyImageView(device_.device(), textureImageView_, nullptr);
    vkDestroyImage(device_.device(), textureImage_, nullptr);
    vkFreeMemory(device_.device(), textureImageMemory_, nullptr);
  }

  LveTexture(const LveTexture &) = delete;
  LveTexture &operator=(const LveTexture &) = delete;

  static std::unique_ptr<LveTexture> loadTexture(
      LveDevice &device, const std::string &textureFilepath);
  static std::unique_ptr<LveTexture> loadCubeMap(
      LveDevice &device,
      const std::string &front,
      const std::string &back,
      const std::string &up,
      const std::string &down,
      const std::string &right,
      const std::string &left);

  VkImageView imageView() const { return textureImageView_; }
  VkSampler sampler() const { return textureSampler_; }
  void updateDescriptor();

  VkDescriptorImageInfo descriptor;

 private:
  void createTextureImage(const std::string &filepath);
  void createTextureImageView(VkImageViewType viewType);
  void createTextureSampler();

  void createCubeTextureImage(
      const std::string &front,
      const std::string &back,
      const std::string &up,
      const std::string &down,
      const std::string &right,
      const std::string &left);

  LveDevice &device_;
  VkImage textureImage_;
  VkDeviceMemory textureImageMemory_;
  VkImageView textureImageView_;
  VkSampler textureSampler_;
  VkImageLayout textureLayout_;
  uint32_t mipLevels_{1};
  uint32_t layerCount_{1};
};

}  // namespace lve
