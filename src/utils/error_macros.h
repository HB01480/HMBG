#pragma once


#define APP_ERROR_RETURN_SDL(errorCondition, errorText, handleErrorLabel)\
if ((errorCondition)){\
    SDL_Log("%s: %s", errorText, SDL_GetError());\
    goto handleErrorLabel;\
}
