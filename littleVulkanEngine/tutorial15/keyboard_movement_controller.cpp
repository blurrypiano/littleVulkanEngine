#include "keyboard_movement_controller.hpp"

// std
#include <limits>

namespace lve {

void KeyboardMovementController::moveInPlaneXZ(Input input, float dt, LveGameObject& gameObject) {
  glm::vec3 rotate{0};

  if (input.getButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
    rotate.y -= input.getMousePositionOffset().x;
    rotate.x += input.getMousePositionOffset().y;
  } else {
    rotate.y += (float)(input.getKeyDown(keys.lookRight) - input.getKeyDown(keys.lookLeft));
    rotate.x += (float)(input.getKeyDown(keys.lookUp) - input.getKeyDown(keys.lookDown));
  }

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
  }

  // limit pitch values between about +/- 85ish degrees
  gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
  gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

  float yaw = gameObject.transform.rotation.y;
  const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
  const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
  const glm::vec3 upDir{0.f, -1.f, 0.f};

  glm::vec3 moveDir{0.f};

  moveDir += forwardDir *
             (float)(input.getKeyDown(keys.moveForward) - input.getKeyDown(keys.moveBackward));
  moveDir += rightDir * (float)(input.getKeyDown(keys.moveRight) - input.getKeyDown(keys.moveLeft));
  moveDir += upDir * (float)(input.getKeyDown(keys.moveUp) - input.getKeyDown(keys.moveDown));

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
  }
}
}  // namespace lve