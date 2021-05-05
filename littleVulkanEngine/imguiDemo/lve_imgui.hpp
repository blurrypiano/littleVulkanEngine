#pragma once

#include "lve_device.hpp"
#include "lve_window.hpp"

// libs
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

// std
#include <stdexcept>

// This whole class is only necessary right now because it needs to manage the descriptor pool
// because we haven't set one up anywhere else in the application, and we manage the
// example state, otherwise all the functions could just be static helper functions if you prefered
namespace lve {

static void check_vk_result(VkResult err) {
  if (err == 0) return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0) abort();
}

class LveImgui {
 public:
  LveImgui(LveWindow &window, LveDevice &device, VkRenderPass renderPass, uint32_t imageCount);
  ~LveImgui();

  void newFrame();

  void render(VkCommandBuffer commandBuffer);

  // Example state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  void runExample();

 private:
  LveDevice &lveDevice;

  // We haven't yet covered descriptor pools in the tutorial series
  // so I'm just going to create one for just imgui and store it here for now.
  // maybe its preferred to have a separate descriptor pool for imgui anyway,
  // I haven't looked into imgui best practices at all.
  VkDescriptorPool descriptorPool;
};
}  // namespace lve
