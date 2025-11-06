#include "clock.h"


void clockTick(Clock *clock) {
    u64 now = SDL_GetTicks();

    clock->dt = (f32)(now - clock->last) / 1000.0f;
    clock->last = now;
}
