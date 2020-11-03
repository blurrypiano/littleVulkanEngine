#version 450

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inView;
layout (location = 2) in vec3 inLight;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 cr = vec3(0.1, 0.1, 0.8); // diffuse reflectance
    vec3 cl = vec3(0.7); // light intensity
    vec3 ca = vec3(0.2); // ambient light
    vec3 cp = vec3(0.35); // highlight color
    float p = 32.0; // phong exponent

    // For metallic objects highlights take on a metallic color

	  vec3 N = normalize(inNormal);
    vec3 L = normalize(inLight);
    vec3 V = normalize(inView);
    vec3 R = reflect(-L, N);

    vec3 diffuse = cr * min(ca + cl * max(dot(N, L), 0.0), vec3(1.0));
    vec3 specular = cl * cp * pow(max(dot(R, V), 0.0), p);

	  outColor = vec4(diffuse + specular, 1.0);
}