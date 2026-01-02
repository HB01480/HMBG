#include "clock.h"


Clock clock_init() {
    Clock clock; SDL_zero(clock);
    clock.dt = 0.0f;
    clock.last = 0ull;

    clock_tick(&clock);
    return clock;
}

void clock_tick(Clock *clock) {
    u64 now = SDL_GetTicks();

    clock->dt = (f32)(now - clock->last) / 1000.0f;
    clock->last = now;
}
