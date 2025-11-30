#version 460

layout (location = 0) in vec3 vPosition;

layout (std140, set = 1, binding = 0) uniform BasicUBO {
    mat4 model;
    mat4 view;
    mat4 projection;

    float time;
};


void main() {
    gl_Position = model * view * projection * vec4(vPosition, 1.0f);
}
