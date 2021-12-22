#include "lve_renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

LveRenderer::LveRenderer(LveWindow& window, LveDevice& device)
    : lveWindow{window}, lveDevice{device} {
  recreateSwapChain();
  createCommandBuffers();
}

LveRenderer::~LveRenderer() { freeCommandBuffers(); }

void LveRenderer::recreateSwapChain() {
  auto extent = lveWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = lveWindow.getExtent();
    glfwWaitEvents();
  }

  lveDevice.device().waitIdle();

  if (lveSwapChain == nullptr) {
    lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
  } else {
    std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain);
    lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, oldSwapChain);

    if (!oldSwapChain->compareSwapFormats(*lveSwapChain.get())) {
      throw std::runtime_error("Swap chain image(or depth) format has changed!");
    }
  }
}

void LveRenderer::createCommandBuffers() {
  commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

  vk::CommandBufferAllocateInfo allocInfo{};
  allocInfo.level = vk::CommandBufferLevel::ePrimary;
  allocInfo.commandPool = lveDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  commandBuffers = lveDevice.device().allocateCommandBuffers(allocInfo);
}

void LveRenderer::freeCommandBuffers() {
  lveDevice.device().freeCommandBuffers(lveDevice.getCommandPool(), commandBuffers);
  commandBuffers.clear();
}

vk::CommandBuffer LveRenderer::beginFrame() {
  assert(!isFrameStarted && "Can't call beginFrame while already in progress");

  auto result = lveSwapChain->acquireNextImage();
  if (result.result == vk::Result::eErrorOutOfDateKHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  currentImageIndex = result.value;

  isFrameStarted = true;

  auto commandBuffer = getCurrentCommandBuffer();
  vk::CommandBufferBeginInfo beginInfo{};

  commandBuffer.begin(beginInfo);

  return commandBuffer;
}

void LveRenderer::endFrame() {
  assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();

  commandBuffer.end();

  auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
  if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR ||
      lveWindow.wasWindowResized()) {
    lveWindow.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != vk::Result::eSuccess) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  isFrameStarted = false;
  currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void LveRenderer::beginSwapChainRenderPass(vk::CommandBuffer commandBuffer) {
  assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't begin render pass on command buffer from a different frame");

  vk::RenderPassBeginInfo renderPassInfo{};
  renderPassInfo.renderPass = lveSwapChain->getRenderPass();
  renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex);

  renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
  renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

  std::array<vk::ClearValue, 2> clearValues{};
  clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{0.01f, 0.01f, 0.01f, 1.0f});
  clearValues[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

  vk::Viewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vk::Rect2D scissor{{0, 0}, lveSwapChain->getSwapChainExtent()};
  commandBuffer.setViewport(0, viewport);
  commandBuffer.setScissor(0, scissor);
}

void LveRenderer::endSwapChainRenderPass(vk::CommandBuffer commandBuffer) {
  assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't end render pass on command buffer from a different frame");
  commandBuffer.endRenderPass();
}

}  // namespace lve
