#include "clock.hh"


void Clock::tick() {
    u64 now = SDL_GetTicks();
    dt = (f32)(now - last) / 1000.0f;

    last = now;
}
