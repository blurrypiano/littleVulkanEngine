#pragma once

#include "rendering/lve_descriptors.hpp"
#include "rendering/lve_device.hpp"
#include "rendering/lve_frame_info.hpp"
#include "rendering/lve_game_object.hpp"
#include "rendering/lve_renderer.hpp"
#include "rendering/lve_window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {

#define MAX_LIGHTS 10

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};  // w is intensity
};

struct GlobalUbo {
  glm::mat4 projectionMatrix{1.f};
  glm::mat4 viewMatrix{1.f};
  glm::mat4 invViewMatrix{1.f};
  glm::vec3 ambientLightColor{0.02f};
  int numLights;
  PointLight lights[MAX_LIGHTS];
};

class FirstApp {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();
  void update(GlobalUbo &ubo, FrameInfo &frameInfo);

 private:
  void loadGameObjects();

  LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  // note: order of declarations matters
  std::unique_ptr<LveDescriptorPool> globalPool{};
  LveGameObject::Map gameObjects;
};
}  // namespace lve
