/*
 * LveApp class
 *
 * Starting point for a little vulkan engine application
 *
 * Copyright (C) 2020 by Blurrypiano - https://github.com/blurrypiano/littleVulkanEngine
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "lve_app.hpp"

// std
#include <array>

namespace lve {

LveApp::LveApp() {
  LveModel::Builder builder{};
  builder.vertices = {{{0.0f, -0.5f, 0.0f}}, {{0.5f, 0.5f, 0.0f}}, {{-0.5f, 0.5f, 0.0f}}};
  builder.indices = {0, 1, 2};
  model_ = std::make_unique<LveModel>(device_, builder);
  createCommandBuffers();
}

LveApp::~LveApp() {
  vkFreeCommandBuffers(
      device_.device(),
      device_.getCommandPool(),
      static_cast<uint32_t>(commandBuffers_.size()),
      commandBuffers_.data());
}

void LveApp::run() {
  while (!window_.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(device_.device());
}

void LveApp::createCommandBuffers() {
  commandBuffers_.resize(swapChain_.imageCount());
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = device_.getCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)commandBuffers_.size();

  if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  for (size_t i = 0; i < commandBuffers_.size(); i++) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                   // Optional
    beginInfo.pInheritanceInfo = nullptr;  // Optional

    if (vkBeginCommandBuffer(commandBuffers_[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain_.getRenderPass();
    renderPassInfo.framebuffer = swapChain_.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain_.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers_[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    pipeline_.bind(commandBuffers_[i]);

    model_->bind(commandBuffers_[i]);
    model_->draw(commandBuffers_[i]);

    vkCmdEndRenderPass(commandBuffers_[i]);
    if (vkEndCommandBuffer(commandBuffers_[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }
}

void LveApp::drawFrame() {
  uint32_t imageIndex;
  auto result = swapChain_.acquireNextImage(&imageIndex);
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  result = swapChain_.submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

}  // namespace lve
