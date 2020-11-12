#pragma once

#include "lve_window.hpp"

namespace lve {

class LveAppBase {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

 private:
  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
};
}  // namespace lve
