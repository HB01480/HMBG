#version 460

layout (location = 0) out vec4 outColor;

layout (std140, set = 3, binding = 0) uniform BasicUBO {
    mat4 model;
    mat4 view;
    mat4 projection;

    float time;
};


void main() {
    float pulse1 = sin(time) * 0.5f + 0.5f;
    float pulse2 = cos(time) * 0.5f + 0.5f;
    float pulse3 = pulse1 * pulse2;
    vec3 baseColor = vec3(0.875f, 0.25f, 0.25f);

    outColor = vec4(baseColor.r * pulse1, baseColor.g * pulse2, baseColor.b * pulse3, 1.0f);
}
