#include "SDLext.h"


void *SDLext_memdup(const void *source, usize size) {
    void *destination = NULL;

    destination = SDL_malloc(size);
    if (!destination) {
        return NULL;
    }

    // Freshly allocated memory doesn't usually overlap with any other already allocated
    SDL_memcpy(destination, source, size);
    return destination;
}
