/*
 * Model Texture class
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_texture.hpp"

// libs
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// std
#include <cmath>
#include <stdexcept>

namespace lve {
std::unique_ptr<LveTexture> LveTexture::loadTexture(
    LveDevice &device, const std::string &textureFilepath) {
  auto modelTexture = std::make_unique<LveTexture>(_constructor_tag{}, device);
  modelTexture->createTextureImage(textureFilepath);
  modelTexture->createTextureImageView(VK_IMAGE_VIEW_TYPE_2D);
  modelTexture->createTextureSampler();
  modelTexture->updateDescriptor();
  return modelTexture;
}

std::unique_ptr<LveTexture> LveTexture::loadCubeMap(
    LveDevice &device,
    const std::string &front,
    const std::string &back,
    const std::string &up,
    const std::string &down,
    const std::string &right,
    const std::string &left) {
  auto modelTexture = std::make_unique<LveTexture>(_constructor_tag{}, device);
  modelTexture->createCubeTextureImage(front, back, up, down, right, left);
  modelTexture->createTextureImageView(VK_IMAGE_VIEW_TYPE_CUBE);
  modelTexture->createTextureSampler();
  modelTexture->updateDescriptor();
  return modelTexture;
}

void LveTexture::updateDescriptor() {
  descriptor.sampler = textureSampler_;
  descriptor.imageView = textureImageView_;
  descriptor.imageLayout = textureLayout_;
}

void LveTexture::createCubeTextureImage(
    const std::string &front,
    const std::string &back,
    const std::string &up,
    const std::string &down,
    const std::string &right,
    const std::string &left) {
  int texWidth, texHeight, texChannels;
  stbi_uc *pixels[6];
  pixels[0] = stbi_load(front.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  pixels[1] = stbi_load(back.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  pixels[2] = stbi_load(up.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  pixels[3] = stbi_load(down.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  pixels[4] = stbi_load(right.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  pixels[5] = stbi_load(left.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

  const VkDeviceSize layerSize = texWidth * texHeight * 4;
  const VkDeviceSize imageSize = layerSize * 6;

  for (int i = 0; i < std::size(pixels); i++) {
    if (!pixels[i]) {
      throw std::runtime_error("failed to load texture image!");
    }
  }

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  device_.createBuffer(
      imageSize,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer,
      stagingBufferMemory);

  void *data;
  vkMapMemory(device_.device(), stagingBufferMemory, 0, imageSize, 0, &data);
  for (int i = 0; i < std::size(pixels); i++) {
    memcpy(static_cast<char *>(data) + (layerSize * i), pixels[i], static_cast<size_t>(layerSize));
  }
  vkUnmapMemory(device_.device(), stagingBufferMemory);
  for (int i = 0; i < std::size(pixels); i++) {
    stbi_image_free(pixels[i]);
  }

  layerCount_ = 6;

  device_.createImage(
      texWidth,
      texHeight,
      mipLevels_,
      VK_FORMAT_R8G8B8A8_SRGB,  // same format as pixels in buffer
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      textureImage_,
      textureImageMemory_,
      VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
      layerCount_);
  device_.transitionImageLayout(
      textureImage_,
      VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      mipLevels_,
      layerCount_);
  device_.copyBufferToImage(
      stagingBuffer,
      textureImage_,
      static_cast<uint32_t>(texWidth),
      static_cast<uint32_t>(texHeight),
      layerCount_);
  device_.transitionImageLayout(
      textureImage_,
      VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      mipLevels_,
      layerCount_);
  textureLayout_ = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
  vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
}

void LveTexture::createTextureImage(const std::string &filepath) {
  int texWidth, texHeight, texChannels;
  stbi_uc *pixels =
      stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  mipLevels_ = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  device_.createBuffer(
      imageSize,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer,
      stagingBufferMemory);

  void *data;
  vkMapMemory(device_.device(), stagingBufferMemory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(device_.device(), stagingBufferMemory);

  stbi_image_free(pixels);

  device_.createImage(
      texWidth,
      texHeight,
      mipLevels_,
      VK_FORMAT_R8G8B8A8_SRGB,  // same format as pixels in buffer
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
          VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      textureImage_,
      textureImageMemory_,
      0,
      layerCount_);

  device_.transitionImageLayout(
      textureImage_,
      VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      mipLevels_,
      layerCount_);
  device_.copyBufferToImage(
      stagingBuffer,
      textureImage_,
      static_cast<uint32_t>(texWidth),
      static_cast<uint32_t>(texHeight),
      layerCount_);
  // device_.transitionImageLayout(
  //     textureImage,
  //     VK_FORMAT_R8G8B8A8_SRGB,
  //     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  //     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  //     mipLevels,
  //     layerCount);

  device_.generateMipmaps(textureImage_, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels_);
  textureLayout_ = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
  vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
}

void LveTexture::createTextureImageView(VkImageViewType viewType) {
  textureImageView_ = device_.createImageView(
      textureImage_,
      VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_ASPECT_COLOR_BIT,
      mipLevels_,
      viewType);
}

void LveTexture::createTextureSampler() {
  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;

  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = 16.0f;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;

  // this fields useful for percentage close filtering for shadow maps
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = static_cast<float>(mipLevels_);

  if (vkCreateSampler(device_.device(), &samplerInfo, nullptr, &textureSampler_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create texture sampler!");
  }
}
}  // namespace lve
