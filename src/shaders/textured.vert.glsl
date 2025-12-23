#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;

layout (location = 0) out vec2 fTexCoord;

layout (std140, set = 1, binding = 0) uniform BasicUBO {
    mat4 model;
    mat4 view;
    mat4 projection;

    float timeSeconds;
};


void main() {
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);

    fTexCoord = vTexCoord;
}
