#pragma once
#include "../common.h"


SDL_GPUShader *createGPUShader(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderStage stage, SDL_GPUShaderFormat format);
