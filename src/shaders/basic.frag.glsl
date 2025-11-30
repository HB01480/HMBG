#version 460

layout (location = 0) out vec4 outColor;

layout (std140, set = 3, binding = 0) uniform BasicUBO {
    mat4 model;
    mat4 view;
    mat4 projection;

    float time;
};


void main() {
    float pulse = sin(time) * 0.5f + 0.5f;
    outColor = vec4(0.875f, 0.25f, 0.25f, 1.0f) * pulse;
}
