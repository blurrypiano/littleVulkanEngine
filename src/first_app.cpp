#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"
#include "lve_camera.hpp"
#include "lve_ubo.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace lve {

FirstApp::FirstApp() {
  globalPool =
      LveDescriptorPool::Builder(lveDevice)
          .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();

  // build frame descriptor pools
  framePools.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
  auto framePoolBuilder = LveDescriptorPool::Builder(lveDevice)
                              .setMaxSets(1000)
                              .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
                              .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
                              .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000);
  for (int i = 0; i < framePools.size(); i++) {
    framePools[i] = framePoolBuilder.build();
  }

  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  LveUbo<GlobalUbo> globalUbo{lveDevice, 1, false, false};
  auto globalSetLayout =
      LveDescriptorSetLayout::Builder(lveDevice)
          .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
          .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = globalUbo.bufferInfoForRegion(i);
    LveDescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{
      lveDevice,
      ecs,
      lveRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  PointLightSystem pointLightSystem{
      lveDevice,
      lveRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout()};
  LveCamera camera{};

  // create a Transform component to store the viewer location
  TransformComponent viewerTransform{};
  viewerTransform.translation.z = -2.5f;

  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerTransform);
    camera.setViewYXZ(viewerTransform.translation, viewerTransform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      int frameIndex = lveRenderer.getFrameIndex();
      framePools[frameIndex]->resetPool();
      FrameInfo frameInfo{
          frameIndex,
          frameTime,
          commandBuffer,
          camera,
          globalDescriptorSets[frameIndex],
          *framePools[frameIndex],
          ecs};

      // update
      GlobalUbo& ubo = globalUbo.get(frameIndex);
      ubo.projection = camera.getProjection();
      ubo.view = camera.getView();
      ubo.inverseView = camera.getInverseView();
      pointLightSystem.update(frameInfo, ubo);
      // globalUbo.write(ubo, frameIndex);
      globalUbo.flushRegion(frameIndex);

      // render
      lveRenderer.beginSwapChainRenderPass(commandBuffer);

      // order here matters
      simpleRenderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);

      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<LveModel> lveModel =
      LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");

  auto flatVase = ecs.createEnt();
  ecs.add<TransformComponent, ModelComponent>(flatVase);
  auto& flatVaseTransform = ecs.get<TransformComponent>(flatVase);
  auto& flatVaseModel = ecs.get<ModelComponent>(flatVase);
  flatVaseTransform.translation = {-.5f, .5f, 0.f};
  flatVaseTransform.scale = {3.f, 1.5f, 3.f};
  flatVaseModel.model = lveModel;

  lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
  auto smothVase = ecs.createEnt();
  ecs.add<TransformComponent, ModelComponent>(smothVase);
  auto& smothVaseTransform = ecs.get<TransformComponent>(smothVase);
  auto& smothVaseModel = ecs.get<ModelComponent>(smothVase);
  smothVaseTransform.translation = {.5f, .5f, 0.f};
  smothVaseTransform.scale = {3.f, 1.5f, 3.f};
  smothVaseModel.model = lveModel;

  lveModel = LveModel::createModelFromFile(lveDevice, "models/quad.obj");
  auto floor = ecs.createEnt();
  ecs.add<TransformComponent, ModelComponent>(floor);
  auto& floorTransform = ecs.get<TransformComponent>(floor);
  auto& floorModel = ecs.get<ModelComponent>(floor);
  floorTransform.translation = {0.f, .5f, 0.f};
  floorTransform.scale = {3.f, 1.f, 3.f};
  floorModel.model = lveModel;

  std::vector<glm::vec3> lightColors{
      {1.f, .1f, .1f},
      {.1f, .1f, 1.f},
      {.1f, 1.f, .1f},
      {1.f, 1.f, .1f},
      {.1f, 1.f, 1.f},
      {1.f, 1.f, 1.f}  //
  };

  for (int i = 0; i < lightColors.size(); i++) {
    auto pointLight = makePointLight(ecs, 0.2f);
    auto& pointLightColor = ecs.get<ColorComponent>(pointLight);
    auto& pointLightTransform = ecs.get<TransformComponent>(pointLight);
    pointLightColor.color = lightColors[i];
    auto rotateLight = glm::rotate(
        glm::mat4(1.f),
        (i * glm::two_pi<float>()) / lightColors.size(),
        {0.f, -1.f, 0.f});
    pointLightTransform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
  }
}

}  // namespace lve
