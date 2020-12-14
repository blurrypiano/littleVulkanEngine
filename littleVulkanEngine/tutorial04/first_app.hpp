#pragma once

#include "lve_pipeline.hpp"
#include "lve_window.hpp"

namespace lve {
class FirstApp {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

 private:
  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  LveDevice lveDevice{lveWindow};
  VkPipelineLayout pipelineLayout;
  LvePipeline lvePipeline{
      lveDevice,
      "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv",
      LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
}  // namespace lve