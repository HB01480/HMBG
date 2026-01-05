#pragma once


#define HANDLE_SDL_ERROR(errorCondition, errorText, handleErrorLabel)\
if ((errorCondition)){\
    SDL_Log("%s: %s: %s", __func__, errorText, SDL_GetError());\
    goto handleErrorLabel;\
}

#define HANDLE_ERROR(errorCondition, errorText, handleErrorLabel)\
if ((errorCondition)){\
    SDL_Log("%s: %s", __func__, errorText);\
    goto handleErrorLabel;\
}
