#include "first_app.hpp"

#include "lve_imgui.hpp"
#include "simple_render_system.hpp"

// libs
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  // create imgui, and pass in dependencies
  LveImgui lveImgui{
      lveWindow,
      lveDevice,
      lveRenderer.getSwapChainRenderPass(),
      lveRenderer.getImageCount()};

  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      // tell imgui that we're starting a new frame
      lveImgui.newFrame();

      lveRenderer.beginSwapChainRenderPass(commandBuffer);

      // render game objects first, so they will be rendered in the background. This
      // is the best we can do for now.
      // Once we cover offscreen rendering, we can render the scene to a image/texture rather than
      // directly to the swap chain. This texture of the scene can then be rendered to an imgui
      // subwindow
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);

      // example code telling imgui what windows to render, and their contents
      // this can be replaced with whatever code/classes you set up configuring your
      // desired engine UI
      lveImgui.runExample();

      // as last step in render pass, record the imgui draw commands
      lveImgui.render(commandBuffer);

      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

  auto triangle = LveGameObject::createGameObject();
  triangle.model = lveModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

}  // namespace lve
