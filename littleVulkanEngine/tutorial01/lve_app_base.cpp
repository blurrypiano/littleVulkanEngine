
#include "lve_app_base.hpp"

namespace lve {

void LveAppBase::run() {
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
  }
}
}  // namespace lve
