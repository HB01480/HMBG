#include "clock.h"


Clock Clock_init() {
    Clock clock; SDL_zero(clock);
    clock.dt = 0.0f;
    clock.last = 0ull;

    Clock_tick(&clock);
    return clock;
}

void Clock_tick(Clock *clock) {
    u64 now = SDL_GetTicks();

    clock->dt = (f32)(now - clock->last) / 1000.0f;
    clock->last = now;
}
