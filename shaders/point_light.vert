#version 450

const vec3 OFFSETS[6] = vec3[](
  vec3(-0.5, -0.5, 0.0),
  vec3(-0.5, 0.5, 0.0),
  vec3(0.5, -0.5, 0.0),
  vec3(0.5, -0.5, 0.0),
  vec3(-0.5, 0.5, 0.0),
  vec3(0.5, 0.5, 0.0)
);

struct PointLight {
  vec4 position; // ignore w
  vec4 color;  // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projectionMatrix;
  mat4 viewMatrix;
  mat4 invViewMatrix; 
  vec3 ambientLightColor;
  int numLights;
  PointLight pointLights[10];
} ubo;

layout(push_constant) uniform Push {
  vec4 position;
  vec4 color;  // w is intensity
  float radius;
} push;

layout(location = 0) out vec2 fragOffset;

void main() {
  vec3 offset = OFFSETS[gl_VertexIndex];
  fragOffset = 2.0 * offset.xy;
  vec3 cameraRightWorld = {ubo.viewMatrix[0][0], ubo.viewMatrix[1][0], ubo.viewMatrix[2][0]};
  vec3 cameraUpWorld = {ubo.viewMatrix[0][1], ubo.viewMatrix[1][1], ubo.viewMatrix[2][1]};

  vec3 positionWorld = push.position.xyz
    + offset.x * 2.0 * push.radius * cameraRightWorld
    + offset.y * 2.0 * push.radius * cameraUpWorld;

  gl_Position = ubo.projectionMatrix * ubo.viewMatrix * vec4(positionWorld, 1.0);
}
