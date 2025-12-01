#include "mouseState.h"


void mouseState_update(MouseState *state) {
    SDL_MouseButtonFlags flags = SDL_GetMouseState(&state->position.x, &state->position.y);
    
    state->leftClick = flags & SDL_BUTTON_LMASK;
    state->middleClick = flags & SDL_BUTTON_MMASK;
    state->rightClick = flags & SDL_BUTTON_RMASK;
}
