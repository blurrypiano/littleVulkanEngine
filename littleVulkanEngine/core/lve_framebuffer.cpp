
#include "lve_framebuffer.hpp"

#include <array>
#include <cassert>
#include <vector>

namespace lve {

// Create a frame buffer attachment

LveFramebuffer::LveFramebuffer(
    LveDevice& device,
    VkRenderPass& renderPass,
    uint32_t width,
    uint32_t height,
    std::vector<AttachmentDescription>& attachmentDescriptions)
    : device{device}, renderPass{renderPass}, width{width}, height{height} {
  attachments.resize(attachmentDescriptions.size());
  for (int i = 0; i < attachmentDescriptions.size(); i++) {
    createAttachment(
        attachmentDescriptions[i].format,
        attachmentDescriptions[i].usage,
        &attachments[i]);
  }
  createFramebuffer();
}

LveFramebuffer::~LveFramebuffer() {
  for (auto& attachment : attachments) {
    vkDestroyImageView(device.device(), attachment.view, nullptr);
    vkDestroyImage(device.device(), attachment.image, nullptr);
    vkFreeMemory(device.device(), attachment.memory, nullptr);
    vkDestroySampler(device.device(), attachment.sampler, nullptr);
  }

  vkDestroyFramebuffer(device.device(), framebuffer, nullptr);
}

// Override framebuffer setup from base class
void LveFramebuffer::createFramebuffer() {
  assert(attachments.size() > 0 && "cannot create framebuffer with no attachments");
  std::vector<VkImageView> views{};
  for (auto& attachment : attachments) {
    views.push_back(attachment.view);
  }

  VkFramebufferCreateInfo framebufferInfo = {};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = renderPass;
  framebufferInfo.attachmentCount = static_cast<uint32_t>(views.size());
  framebufferInfo.pAttachments = views.data();
  framebufferInfo.width = width;
  framebufferInfo.height = height;
  framebufferInfo.layers = 1;

  if (vkCreateFramebuffer(device.device(), &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create framebuffer!");
  }
}

void LveFramebuffer::createAttachment(
    VkFormat format, VkImageUsageFlags usage, FramebufferAttachment* attachment) {
  VkImageAspectFlags aspectMask = 0;
  VkImageLayout imageLayout;

  attachment->format = format;

  if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
    aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  }
  if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  }

  // I hate this!
  VkImageCreateInfo imageInfo = lve::initializers::imageCreateInfo();
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.format = format;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.usage = usage;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  device.createImageWithInfo(
      imageInfo,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      attachment->image,
      attachment->memory);

  VkImageViewCreateInfo viewInfo = lve::initializers::imageViewCreateInfo();
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange = {};
  viewInfo.subresourceRange.aspectMask = aspectMask;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;
  viewInfo.image = attachment->image;
  if (vkCreateImageView(device.device(), &viewInfo, nullptr, &attachment->view) != VK_SUCCESS) {
    throw std::runtime_error("failed to create texture image view!");
  }

  if (usage & VK_IMAGE_USAGE_SAMPLED_BIT) {
    // Create sampler to sample from the attachment in the fragment shader
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = samplerInfo.addressModeU;
    samplerInfo.addressModeW = samplerInfo.addressModeU;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    if (vkCreateSampler(device.device(), &samplerInfo, nullptr, &attachment->sampler) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to create sampler!");
    }

    VkImageLayout samplerImageLayout = imageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                           ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                                           : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    attachment->descriptor.imageLayout = samplerImageLayout;
    attachment->descriptor.imageView = attachment->view;
    attachment->descriptor.sampler = attachment->sampler;
  }
}

void LveFramebuffer::Builder::addAttachment(VkFormat format, VkImageUsageFlags usage) {
  attachmentDescriptions.push_back({format, usage});
}

std::unique_ptr<LveFramebuffer> LveFramebuffer::Builder::build(
    LveDevice& device, VkRenderPass& renderPass) {
  return std::make_unique<LveFramebuffer>(
      device,
      renderPass,
      width,
      height,
      attachmentDescriptions);
}

}  // namespace lve