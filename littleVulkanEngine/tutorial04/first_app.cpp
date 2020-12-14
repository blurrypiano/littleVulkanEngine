#include "first_app.hpp"

namespace lve {

void FirstApp::run() {
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
  }
}
}  // namespace lve