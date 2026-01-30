#pragma once
#include <SDL3/SDL.h>


#define CONDITIONAL_PRINT(condition, ...) if ((condition)) {\
    SDL_Log(__VA_ARGS__);\
}

#define CONDITONAL_GOTO(condition, handleLabel) if ((condition)) {\
    goto handleLabel;\
}

#define CONDITIONAL_NOTIFYING_PRINT(condition, handleLabel, ...) if ((condition)) {\
    SDL_Log(__VA_ARGS__);\
    goto handleLabel;\
}

#define HANDLE_CRITICAL(condition, handleLabel, text) CONDITIONAL_NOTIFYING_PRINT(condition, handleLabel, "CRITICAL(%s:%i): %s", __FILE_NAME__, __LINE__, text)
#define HANDLE_ERROR(condition, handleLabel, text) CONDITIONAL_NOTIFYING_PRINT(condition, handleLabel, "ERROR(%s:%i): %s", __FILE_NAME__, __LINE__, text)
#define CHECK_ERROR(condition, text) CONDITIONAL_PRINT(condition, "ERROR(%s:%i): %s", __FILE_NAME__, __LINE__, text)
#define CHECK_WARNING(condition, text) CONDITIONAL_PRINT(condition, "WARNING(%s:%i): %s", __FILE_NAME__, __LINE__, text)

#define HANDLE_SDL_CRITICAL(condition, handleLabel, text) CONDITIONAL_NOTIFYING_PRINT(condition, handleLabel, "CRITICAL(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError())
#define HANDLE_SDL_ERROR(condition, handleLabel, text) CONDITIONAL_NOTIFYING_PRINT(condition, handleLabel, "ERROR(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError())
#define CHECK_SDL_ERROR(condition, text) CONDITIONAL_PRINT(condition, "ERROR(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError())
#define CHECK_SDL_WARNING(condition, text) CONDITIONAL_PRINT(condition, "WARNING(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError())
