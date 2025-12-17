#pragma once
#include "../common.h"


SDL_GPUShader *createGPUShaderFromFilepath(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderStage stage, SDL_GPUShaderFormat format, u32 numSamplers, u32 numStorageBuffers, u32 numStorageTextures, u32 numUniformBuffers);

SDL_GPUShader *createGPUVertexShaderFromFilepath(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderFormat format, u32 numSamplers, u32 numStorageBuffers, u32 numStorageTextures, u32 numUniformBuffers);
SDL_GPUShader *createGPUFragmentShaderFromFilepath(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderFormat format, u32 numSamplers, u32 numStorageBuffers, u32 numStorageTextures, u32 numUniformBuffers);
