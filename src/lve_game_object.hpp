#pragma once

#include "lve_model.hpp"
#include "lve_swap_chain.hpp"
#include "lve_texture.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace lve {

struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
};

struct PointLightComponent {
  float lightIntensity = 1.0f;
};

struct GameObjectBufferData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

class LveGameObjectManager;  // forward declare game object manager class

class LveGameObject {
 public:
  using id_t = unsigned int;
  using Map = std::unordered_map<id_t, LveGameObject>;

  LveGameObject(LveGameObject &&) = default;
  LveGameObject(const LveGameObject &) = delete;
  LveGameObject &operator=(const LveGameObject &) = delete;
  LveGameObject &operator=(LveGameObject &&) = delete;

  id_t getId() { return id; }

  VkDescriptorBufferInfo getBufferInfo(int frameIndex);

  glm::vec3 color{};
  TransformComponent transform{};

  // Optional pointer components
  std::shared_ptr<LveModel> model{};
  std::shared_ptr<LveTexture> diffuseMap = nullptr;
  std::unique_ptr<PointLightComponent> pointLight = nullptr;

 private:
  LveGameObject(id_t objId, const LveGameObjectManager &manager);

  id_t id;
  const LveGameObjectManager &gameObjectManger;

  friend class LveGameObjectManager;
};

class LveGameObjectManager {
 public:
  static constexpr int MAX_GAME_OBJECTS = 1000;

  LveGameObjectManager(LveDevice &device);
  LveGameObjectManager(const LveGameObjectManager &) = delete;
  LveGameObjectManager &operator=(const LveGameObjectManager &) = delete;
  LveGameObjectManager(LveGameObjectManager &&) = delete;
  LveGameObjectManager &operator=(LveGameObjectManager &&) = delete;

  LveGameObject &createGameObject() {
    assert(currentId < MAX_GAME_OBJECTS && "Max game object count exceeded!");
    auto gameObject = LveGameObject{currentId++, *this};
    auto gameObjectId = gameObject.getId();
    gameObject.diffuseMap = textureDefault;
    gameObjects.emplace(gameObjectId, std::move(gameObject));
    return gameObjects.at(gameObjectId);
  }

  LveGameObject &makePointLight(
      float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

  VkDescriptorBufferInfo getBufferInfoForGameObject(
      int frameIndex, LveGameObject::id_t gameObjectId) const {
    return uboBuffers[frameIndex]->descriptorInfoForIndex(gameObjectId);
  }

  void updateBuffer(int frameIndex);

  LveGameObject::Map gameObjects{};
  std::vector<std::unique_ptr<LveBuffer>> uboBuffers{LveSwapChain::MAX_FRAMES_IN_FLIGHT};

 private:
  LveGameObject::id_t currentId = 0;
  std::shared_ptr<LveTexture> textureDefault;
};

// template <typename T>
// class UboArraySystem {
//  public:
//   static constexpr int SIZE = 1000;

//   UboArraySystem(LveDevice &device, int length) {
//     // including nonCoherentAtomSize allows us to flush a specific index at once
//     int alignment = std::lcm(
//         device.properties.limits.nonCoherentAtomSize,
//         device.properties.limits.minUniformBufferOffsetAlignment);
//     for (int i = 0; i < uboBuffers.size(); i++) {
//       uboBuffers[i] = std::make_unique<LveBuffer>(
//           device,
//           sizeof(T),
//           length,
//           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
//           alignment);
//       uboBuffers[i]->map();
//     }
//   }

//   UboArraySystem(const UboArraySystem &) = delete;
//   UboArraySystem &operator=(const UboArraySystem &) = delete;
//   UboArraySystem(UboArraySystem &&) = delete;
//   UboArraySystem &operator=(UboArraySystem &&) = delete;

//   void updateBuffer(FrameInfo &frameInfo);  // query all with Transform component, write
//   VkDescriptorBufferInfo getDescriptorInfo(Ent::id_t entID) const;

//   // vector of pairs that we iterator over? or unordered map
//   // if we're always writing all entries than
//   std::unordered_map<Ent::id_t, int> entIndexMap{};
//   std::vector<std::unique_ptr<LveBuffer>> uboBuffers{LveSwapChain::MAX_FRAMES_IN_FLIGHT};

//  private:
// };

}  // namespace lve
