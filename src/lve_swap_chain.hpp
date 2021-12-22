#pragma once

#include "lve_device.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace lve {

class LveSwapChain {
 public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  LveSwapChain(LveDevice &deviceRef, vk::Extent2D windowExtent);
  LveSwapChain(
      LveDevice &deviceRef, vk::Extent2D windowExtent, std::shared_ptr<LveSwapChain> previous);

  ~LveSwapChain();

  LveSwapChain(const LveSwapChain &) = delete;
  LveSwapChain &operator=(const LveSwapChain &) = delete;

  vk::Framebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
  vk::RenderPass getRenderPass() { return renderPass; }
  vk::ImageView getImageView(int index) { return swapChainImageViews[index]; }
  size_t imageCount() { return swapChainImages.size(); }
  vk::Format getSwapChainImageFormat() { return swapChainImageFormat; }
  vk::Extent2D getSwapChainExtent() { return swapChainExtent; }
  uint32_t width() { return swapChainExtent.width; }
  uint32_t height() { return swapChainExtent.height; }

  float extentAspectRatio() {
    return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
  }
  vk::Format findDepthFormat();

  vk::ResultValue<uint32_t> acquireNextImage();
  vk::Result submitCommandBuffers(const vk::CommandBuffer *buffers, uint32_t *imageIndex);

  bool compareSwapFormats(const LveSwapChain &swapChain) const {
    return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
           swapChain.swapChainImageFormat == swapChainImageFormat;
  }

 private:
  void init();
  void createSwapChain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  // Helper functions
  vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR> &availableFormats);
  vk::PresentModeKHR chooseSwapPresentMode(
      const std::vector<vk::PresentModeKHR> &availablePresentModes);
  vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

  vk::Format swapChainImageFormat;
  vk::Format swapChainDepthFormat;
  vk::Extent2D swapChainExtent;

  std::vector<vk::Framebuffer> swapChainFramebuffers;
  vk::RenderPass renderPass;

  std::vector<vk::Image> depthImages;
  std::vector<vk::DeviceMemory> depthImageMemorys;
  std::vector<vk::ImageView> depthImageViews;
  std::vector<vk::Image> swapChainImages;
  std::vector<vk::ImageView> swapChainImageViews;

  LveDevice &device;
  vk::Extent2D windowExtent;

  vk::SwapchainKHR swapChain;
  std::shared_ptr<LveSwapChain> oldSwapChain;

  std::vector<vk::Semaphore> imageAvailableSemaphores;
  std::vector<vk::Semaphore> renderFinishedSemaphores;
  std::vector<vk::Fence> inFlightFences;
  std::vector<vk::Fence> imagesInFlight;
  size_t currentFrame = 0;
};

}  // namespace lve
