#pragma once
#include <SDL3/SDL.h>

#include "typedefs.h"


void *SDLext_memdup(const void *source, usize size);

int SDLext_memcmp(const void *mem1, const void *mem2, usize size);

void *SDLext_LoadStorageFile(usize *outBufferSize, SDL_Storage *storage, const char *filepath);
