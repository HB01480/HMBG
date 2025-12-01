#pragma once
#include "../common.h"


typedef struct MouseState {
    vec2s position;

    bool leftClick;
    bool middleClick;
    bool rightClick;

} MouseState;


void mouseState_update(MouseState *state);
