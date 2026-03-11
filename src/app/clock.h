#pragma once
#include "../common.h"


typedef struct Clock {
    f32 dt;
    u64 last;

} Clock;


Clock Clock_init();
void Clock_tick(Clock *clock);
