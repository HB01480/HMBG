#version 460

layout (location = 0) in vec2 fTexCoord;

layout (location = 0) out vec4 outColor;

layout (set = 2, binding = 1) uniform sampler2D diffuseSampler;

layout (std140, set = 3, binding = 0) uniform BasicUBO {
    mat4 model;
    mat4 view;
    mat4 projection;

    float timeSeconds;
};


void main() {
    outColor = texture(diffuseSampler, fTexCoord);
}
