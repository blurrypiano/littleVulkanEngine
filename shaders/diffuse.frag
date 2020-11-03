#version 450

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inLight;
layout (location = 2) in vec3 inColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 cr = inColor; // diffuse reflectance
    vec3 cl = vec3(0.7); // light intensity
    vec3 ca = vec3(0.2); // ambient light

    vec3 N = normalize(inNormal);
    vec3 L = normalize(inLight);
    vec3 diffuse = cr * min(ca + cl * max(dot(N, L), 0.0), vec3(1.0));

    outColor = vec4(diffuse, 1.0);
}