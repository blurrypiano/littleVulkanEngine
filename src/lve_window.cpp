#include "lve_window.hpp"

// std
#include <stdexcept>

namespace lve {

LveWindow::LveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
  initWindow();
}

LveWindow::~LveWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void LveWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

const vk::SurfaceKHR &LveWindow::createWindowSurface(vk::Instance instance) {
  const auto result = static_cast<vk::Result>(
      glfwCreateWindowSurface(VkInstance(instance), window, nullptr, &surfaceC));

  if (result != vk::Result::eSuccess) {
    throw std::runtime_error("failed to create window surface!");
  }
  
  cppSurface = vk::SurfaceKHR(surfaceC);
  return cppSurface;
}

void LveWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto lveWindow = reinterpret_cast<LveWindow *>(glfwGetWindowUserPointer(window));
  lveWindow->framebufferResized = true;
  lveWindow->width = width;
  lveWindow->height = height;
}

}  // namespace lve
