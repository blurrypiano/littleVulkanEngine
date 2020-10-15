
#include "lve_app.hpp"

namespace lve {

void LveApp::run() {
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
  }
}
}  // namespace lve
