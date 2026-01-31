#pragma once
#include <SDL3/SDL.h>

#include "print_macros.h"


#define HANDLE_CRITICAL(condition, handleLabel, text) if ((condition)) { PRINT_CRITICAL(text) goto handleLabel; }
#define HANDLE_ERROR(condition, handleLabel, text) if ((condition)) { PRINT_ERROR(text) goto handleLabel; }
#define CHECK_ERROR(condition, text) if ((condition)) { PRINT_ERROR(text) }
#define CHECK_WARNING(condition, text) if ((condition)) { PRINT_WARNING(text) }

#define HANDLE_SDL_CRITICAL(condition, handleLabel, text) if ((condition)) { PRINT_SDL_CRITICAL(text) goto handleLabel; }
#define HANDLE_SDL_ERROR(condition, handleLabel, text) if ((condition)) { PRINT_SDL_ERROR(text) goto handleLabel; }
#define CHECK_SDL_ERROR(condition, text) if ((condition)) { PRINT_SDL_ERROR(text) }
#define CHECK_SDL_WARNING(condition, text) if ((condition)) { PRINT_SDL_WARNING(text) }
