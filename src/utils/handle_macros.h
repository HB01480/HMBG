#pragma once


#define HANDLE_SDL_CRITICAL(failureCondition, criticalText, handleCriticalLabel)\
if ((failureCondition)) {\
    SDL_Log("%s: %s: %s", __func__, criticalText, SDL_GetError());\
    goto handleCriticalLabel;\
}

#define HANDLE_CRITICAL(failureCondition, criticalText, handleCriticalLabel)\
if ((failureCondition)) {\
    SDL_Log("%s: %s", __func__, criticalText);\
    goto handleCriticalLabel;\
}

#define HANDLE_SDL_ERROR(failureCondition, errorText)\
if ((failureCondition)) {\
    SDL_Log("%s: %s: %s", __func__, errorText, SDL_GetError());\
}

#define HANDLE_ERROR(failureCondition, errorText)\
if ((failureCondition)) {\
    SDL_Log("%s: %s", __func__, errorText);\
}
