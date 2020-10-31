#version 450

layout (location = 0) in vec3 inPosition;

void main() {
  gl_Position = vec4(inPosition, 1.0);
}
