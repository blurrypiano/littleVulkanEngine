#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragNormal;

layout (location = 0) out vec4 outColor;

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
  // single point light
  // vec3 directionToLight = ubo.lightPosition - fragWorldPos;
  // float lightAttenutation = ubo.lightIntensity / dot(directionToLight, directionToLight);
  // vec3 light = ubo.ambientLightColor 
  //   + ubo.lightColor * lightAttenutation * max(dot(normalize(fragNormal), normalize(directionToLight)), 0);

  vec3 diffuseLighting = ubo.ambientLightColor;
  vec3 specularLighting = vec3(0.0);
  vec3 surfaceNormal = normalize(fragNormal);

  vec3 cameraPosWorld = ubo.invViewMatrix[3].xyz;

  for (int i = 0; i < ubo.numLights; i++) {
    PointLight light = ubo.pointLights[i];
    vec3 directionToLight = light.position.xyz - fragWorldPos;
    float attenuation = light.color.w / dot(directionToLight, directionToLight);
    float cosAngIncidence = dot(surfaceNormal, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    // diffuse lighting
    diffuseLighting += light.color.xyz * attenuation * cosAngIncidence;

    // specular lighting
    vec3 viewDirection = normalize(cameraPosWorld - fragWorldPos);
    vec3 halfAngle = normalize(directionToLight + viewDirection);
    float blinnTerm = dot(surfaceNormal, halfAngle);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = cosAngIncidence != 0.0 ? blinnTerm : 0.0;
    blinnTerm = pow(blinnTerm, 32.0);
    specularLighting += light.color.xyz * attenuation * blinnTerm;
  }
  outColor = vec4((specularLighting + diffuseLighting) * fragColor, 1.0);
}
