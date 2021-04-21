/*
 * Encapsulates a vulkan frame buffer, render pass and corresponding attachments, images, views
 * and memory
 *
 * Copyright (C) 2020 by Brendan Galea - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "lve_device.hpp"
#include "lve_initializers.hpp"
#include "lve_swap_chain.hpp"

// std
#include <memory>
#include <vector>

namespace lve {

struct FramebufferAttachment {
  VkImage image;
  VkDeviceMemory memory;
  VkImageView view;
  VkFormat format;
  VkSampler sampler;
  VkDescriptorImageInfo descriptor;
};

class LveFramebuffer {
 public:
  struct AttachmentDescription {
    VkFormat format;
    VkImageUsageFlags usage;
  };

  class Builder {
   public:
    uint32_t width;
    uint32_t height;

    void addAttachment(VkFormat format, VkImageUsageFlags usage);
    std::unique_ptr<LveFramebuffer> build(LveDevice &device, VkRenderPass renderPass);

   private:
    std::vector<AttachmentDescription> attachmentDescriptions;
  };

  LveFramebuffer(
      LveDevice &device,
      VkRenderPass renderPass,
      uint32_t width,
      uint32_t height,
      std::vector<AttachmentDescription> &attachmentDescriptions);

  ~LveFramebuffer();

  LveFramebuffer(const LveFramebuffer &) = delete;
  LveFramebuffer &operator=(const LveFramebuffer &) = delete;

  std::vector<lve::FramebufferAttachment> attachments;
  VkFramebuffer framebuffer;
  const uint32_t width;
  const uint32_t height;

 private:
  void createFramebuffer(VkRenderPass renderPass);
  void createAttachment(
      VkFormat format, VkImageUsageFlags usage, FramebufferAttachment *attachment);

  LveDevice &device;
};
}  // namespace lve