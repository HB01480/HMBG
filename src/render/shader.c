#include "shader.h"


SDL_GPUShader *createGPUShader(SDL_GPUDevice *gpu, SDL_Storage *storage, const char *filepath, SDL_GPUShaderStage stage, SDL_GPUShaderFormat format) {
    void *fileContent = NULL;
    usize fileSize = 0;

    if (!SDL_GetStorageFileSize(storage, filepath, &fileSize)) {
        SDL_Log("Can't get the file size of shader code. File path of '%s'", filepath);
        return NULL;
    }

    fileContent = SDL_malloc(fileSize);
    if (!fileContent) {
        SDL_Log("Failed to allocate memory for shader code. File path of '%s'", filepath);
        return NULL;
    }

    if (!SDL_ReadStorageFile(storage, filepath, fileContent, fileSize)) {
        SDL_Log("Can't read file. File path of '%s'", filepath);
        return NULL;
    }

    SDL_GPUShaderCreateInfo shaderInfo; SDL_zero(shaderInfo);
    shaderInfo.code_size = fileSize;
    shaderInfo.code = fileContent;
    shaderInfo.stage = stage;
    shaderInfo.format = format;
    shaderInfo.num_samplers = 0;
    shaderInfo.num_storage_buffers = 0;
    shaderInfo.num_storage_textures = 0;
    shaderInfo.num_uniform_buffers = 1;

    SDL_GPUShader *shader = SDL_CreateGPUShader(gpu, &shaderInfo);
    if (!shader) {
        SDL_Log("Failed to create gpu shader. File path of '%s'", filepath);
        return NULL;
    }

    return shader;
}
