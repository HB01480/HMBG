#include "common.hh"


void* operator new(usize size) {
    return SDL_malloc(size); 
}

void operator delete(void* ptr) noexcept {
    SDL_free(ptr);
}

void* operator new[](usize size) {
    return SDL_malloc(size); 
}

void operator delete[](void* ptr) noexcept {
    SDL_free(ptr);
}
