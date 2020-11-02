#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragLight;

layout(location = 0) out vec4 outColor;

void main() {
    float brightness = max(dot(fragNormal, fragLight), 0.2);
    outColor = vec4(brightness *  vec3(0.1, 0.1, 0.8), 1.0);
}