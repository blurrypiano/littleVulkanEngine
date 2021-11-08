#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos;
layout(location = 2) out vec3 fragNormal;

struct PointLight {
  vec4 position;
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
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;

void main() {
  vec4 worldPosition = push.modelMatrix * vec4(position, 1.0);
  gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;
  fragNormal = normalize(mat3(push.normalMatrix) * normal);
  fragWorldPos = worldPosition.xyz;
  fragColor = color;
}
