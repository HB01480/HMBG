#version 460

layout (location = 0) in vec2 fTexCoord;

layout (location = 0) out vec4 outColor;

layout (set = 2, binding = 1) uniform sampler2D diffuseSampler;



void main() {
    outColor = texture(diffuseSampler, fTexCoord);
}
