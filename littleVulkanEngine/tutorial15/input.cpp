#include "input.hpp"

#include <chrono>
#include <iostream>

namespace lve {
Input::Input(GLFWwindow *window) : EventEmitter() {
  glfwSetWindowUserPointer(window, this);
  glfwSetKeyCallback(window, &Input::key_callback);
  glfwSetMouseButtonCallback(window, &Input::mouse_button_callback);
  glfwSetScrollCallback(window, &Input::scroll_callback);
  glfwSetCursorPosCallback(window, &Input::cursor_position_callback);
  std::cout << "Input Initialized!" << std::endl;
}

Input::~Input() {}

void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  static_cast<Input *>(glfwGetWindowUserPointer(window))
      ->onKeyboardEvent(key, scancode, action, mods);
}

void Input::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
  static_cast<Input *>(glfwGetWindowUserPointer(window))->onMouseButtonEvent(button, action, mods);
}

void Input::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  static_cast<Input *>(glfwGetWindowUserPointer(window))->onMouseWheelEvent(xoffset, yoffset);
}

void Input::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  static_cast<Input *>(glfwGetWindowUserPointer(window))->onMouseMoveEvent(xpos, ypos);
}

long long _now() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void Input::onKeyboardEvent(int key, int scancode, int action, int mods) {
  long long now = _now();

  if (action == GLFW_PRESS) {
    keyDown[key] = true;
    keyDownTime[key] = now;
    emit("key_down", key, mods);
  } else if (action == GLFW_RELEASE) {
    keyDown[key] = false;

    if (now - keyDownTime[key] < downTimeThreshold) {
      emit("key_pressed", key, mods);
    } else {
      emit("key_up", key, mods);
    }

    keyDownTime[key] = 0;
  }
}

void Input::onMouseButtonEvent(int button, int action, int mods) {
  long long now = _now();

  if (action == GLFW_PRESS) {
    buttonDown[button] = true;
    buttonDownTime[button] = now;
    emit("button_down", button, mods);
  } else if (action == GLFW_RELEASE) {
    buttonDown[button] = false;

    if (now - buttonDownTime[button] < downTimeThreshold) {
      emit("button_clicked", button, mods);
    } else {
      emit("button_up", button, mods);
    }

    buttonDownTime[button] = 0;
  }
}

void Input::onMouseWheelEvent(double xoffset, double yoffset) {
  if (xoffset > 0) {
    emit("wheel_left", xoffset);
  } else if (xoffset < 0) {
    emit("wheel_right", xoffset);
  }

  if (yoffset > 0) {
    emit("wheel_up", yoffset);
  } else if (yoffset < 0) {
    emit("wheel_down", yoffset);
  }
}

void Input::onMouseMoveEvent(double xpos, double ypos) {
  mousePositionOffset.x = mousePosition.x - xpos;
  mousePositionOffset.y = mousePosition.y - ypos;
  mousePosition.x = xpos;
  mousePosition.y = ypos;

  emit("mouse_moved", mousePosition, mousePositionOffset);
}
}  // namespace lve