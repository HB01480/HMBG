#pragma once
#include "../common.h"


typedef struct Clock {
    f32 dt;
    u64 last;

} Clock;


void clock_tick(Clock *clock);
