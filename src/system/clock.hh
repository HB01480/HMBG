#pragma once
#include "../common.hh"


typedef struct Clock {
    f32 dt = 0.0f;
    u64 last = 0ull;

    void tick();

} Clock;
