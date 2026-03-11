#pragma once
#include "../common.h"


typedef struct MouseState {
    vec2s position;

    bool leftClick;
    bool middleClick;
    bool rightClick;

} MouseState;


MouseState MouseState_init();
void MouseState_update(MouseState *state);
