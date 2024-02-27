#pragma once

#include "ecs/lve_ecs.hpp"
#include "lve_model.hpp"

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

struct ColorComponent {
  glm::vec3 color{};
};

struct ModelComponent {
  std::shared_ptr<LveModel> model{};
};

inline EntId makePointLight(
    EntManager& ecs,
    float intensity = 10.f,
    float radius = 0.1f,
    glm::vec3 color = glm::vec3(1.f)) {
  EntId entId = ecs.createEnt();
  // TODO simplify -> unpack component references
  ecs.add<TransformComponent, ColorComponent, PointLightComponent>(entId);

  auto& transformComp = ecs.get<TransformComponent>(entId);
  auto& colorComp = ecs.get<ColorComponent>(entId);
  auto& pointLightComp = ecs.get<PointLightComponent>(entId);

  colorComp.color = color;
  transformComp.scale.x = radius;
  pointLightComp.lightIntensity = intensity;
  return entId;
}

}  // namespace lve
