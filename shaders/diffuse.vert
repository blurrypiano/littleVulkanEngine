#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(set = 0, binding = 0) uniform SystemUbo {
    mat4 projection;
    mat4 modelView;
    vec4 lightPos;
} ubo;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragLight;

void main() {
    gl_Position = ubo.projection * ubo.modelView * vec4(inPosition, 1.0);
    fragNormal = inNormal; // temporary hack, not transforming since right now model matrix will be identity
    fragLight = ubo.lightPos.xyz;
}