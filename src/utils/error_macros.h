#pragma once


#define HANDLE_SDL_ERROR(errorCondition, errorText, handleErrorLabel)\
if ((errorCondition)){\
    SDL_Log("%s: %s", errorText, SDL_GetError());\
    goto handleErrorLabel;\
}
