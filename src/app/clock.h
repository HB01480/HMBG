#pragma once
#include "../common.h"


typedef struct Clock {
    f32 dt;
    u64 last;

} Clock;


Clock clock_init();
void clock_tick(Clock *clock);
