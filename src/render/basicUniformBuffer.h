#pragma once
#include "../common.h"


typedef struct BasicUBO {
    mat4s model, view, projection;

    f32 timeSeconds;

} BasicUBO;
