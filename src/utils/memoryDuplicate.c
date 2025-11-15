#include "memoryDuplicate.h"


void *memoryDuplicate(const void *source, usize size) {
    if (source == NULL || size == 0) return NULL;

    void *destination = SDL_malloc(size);
    if (destination == NULL) return NULL;

    SDL_memcpy(destination, source, size);

    return destination;
}
