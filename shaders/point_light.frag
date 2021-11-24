#version 450

layout(location = 0) in vec2 fragOffset;

layout (location = 0) out vec4 outColor;

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

void main() {
  float alpha = 1.0 - sqrt(dot(fragOffset, fragOffset));
  if (alpha <= 0.0) {
    discard;
  }
  outColor = vec4(2.0 * push.color.xyz * push.color.w, alpha);
}
