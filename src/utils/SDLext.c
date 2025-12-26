#include "SDLext.h"


void *SDLext_memdup(const void *source, usize size) {
    void *destination = NULL;

    destination = SDL_malloc(size);
    if (!destination) {
        return NULL;
    }

    // Freshly allocated memory doesn't usually overlap with any other already allocated memory
    SDL_memcpy(destination, source, size);
    return destination;
}

void *SDLext_LoadStorageFile(usize *outBufferSize, SDL_Storage *storage, const char *filepath) {
    void *buffer = NULL;
    usize bufferSize = 0;

    if (!SDL_GetStorageFileSize(storage, filepath, &bufferSize)) {
        SDL_Log("Failed to get the file size from the '%s' file to be loaded: %s", filepath, SDL_GetError());
        return NULL;
    }

    buffer = SDL_malloc(bufferSize);
    if (!buffer) {
        SDL_Log("Failed to allocate memory for a buffer, '%s' file to be loaded: %s", filepath, SDL_GetError());
        return NULL;
    }

    if (!SDL_ReadStorageFile(storage, filepath, buffer, bufferSize)) {
        SDL_Log("Failed to read file into a buffer, from the '%s' file to be loaded: %s", filepath, SDL_GetError());
        SDL_free(buffer);
        return NULL;
    }

    if (!outBufferSize) *outBufferSize = bufferSize;
    return buffer;
}
