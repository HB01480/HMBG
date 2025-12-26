#include "shader.h"


SDL_GPUShader *createGPUShaderFromFilepath(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderStage stage, SDL_GPUShaderFormat format, u32 numSamplers, u32 numStorageBuffers, u32 numStorageTextures, u32 numUniformBuffers) {
    void *fileContent = NULL;
    usize fileSize = 0;

    fileContent = SDLext_LoadStorageFile(&fileSize, storage, filepath);
    if (!fileContent) {
        SDL_Log("Failed to load shader from '%s' filepath: %s", filepath, SDL_GetError());
        return NULL;
    }

    SDL_GPUShaderCreateInfo shaderInfo; SDL_zero(shaderInfo);
    shaderInfo.code_size = fileSize;
    shaderInfo.code = fileContent;
    shaderInfo.stage = stage;
    shaderInfo.format = format;
    shaderInfo.num_samplers = numSamplers;
    shaderInfo.num_storage_buffers = numStorageBuffers;
    shaderInfo.num_storage_textures = numStorageTextures;
    shaderInfo.num_uniform_buffers = numUniformBuffers;

    SDL_GPUShader *shader = SDL_CreateGPUShader(gpu, &shaderInfo);
    if (!shader) {
        SDL_Log("Failed to create gpu shader. File path of '%s'", filepath);
        return NULL;
    }

    return shader;
}

SDL_GPUShader *createGPUVertexShaderFromFilepath(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderFormat format, u32 numSamplers, u32 numStorageBuffers, u32 numStorageTextures, u32 numUniformBuffers) {
    return createGPUShaderFromFilepath(gpu, storage, filepath, SDL_GPU_SHADERSTAGE_VERTEX, format, numSamplers, numStorageBuffers, numStorageTextures, numUniformBuffers);
}

SDL_GPUShader *createGPUFragmentShaderFromFilepath(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderFormat format, u32 numSamplers, u32 numStorageBuffers, u32 numStorageTextures, u32 numUniformBuffers) {
    return createGPUShaderFromFilepath(gpu, storage, filepath, SDL_GPU_SHADERSTAGE_FRAGMENT, format, numSamplers, numStorageBuffers, numStorageTextures, numUniformBuffers);
}
