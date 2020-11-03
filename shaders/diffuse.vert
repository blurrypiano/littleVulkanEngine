#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(set = 0, binding = 0) uniform UBO {
    mat4 projection;
    mat4 viewing;
    mat4 model;
    mat4 normals;
    vec4 lightPos;
} ubo;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outLight;

void main() {
    vec4 pos = ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.projection * ubo.viewing * pos;
    outNormal =  mat3(ubo.normals) * inNormal;
    outLight =  ubo.lightPos.xyz - pos.xyz;
}