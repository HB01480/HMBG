#pragma once
#include <SDL3/SDL.h>

#include "print_macros.h"


#define HANDLE_CRITICAL(condition, handleLabel, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_CRITICAL(formatLiteral, __VA_ARGS__) goto handleLabel; }
#define HANDLE_ERROR(condition, handleLabel, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_ERROR(formatLiteral, __VA_ARGS__) goto handleLabel; }
#define CHECK_ERROR(condition, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_ERROR(formatLiteral, __VA_ARGS__) }
#define CHECK_WARNING(condition, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_WARNING(formatLiteral, __VA_ARGS__) }

#define HANDLE_SDL_CRITICAL(condition, handleLabel, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_SDL_CRITICAL(formatLiteral, __VA_ARGS__) goto handleLabel; }
#define HANDLE_SDL_ERROR(condition, handleLabel, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_SDL_ERROR(formatLiteral, __VA_ARGS__) goto handleLabel; }
#define CHECK_SDL_ERROR(condition, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_SDL_ERROR(formatLiteral, __VA_ARGS__) }
#define CHECK_SDL_WARNING(condition, formatLiteral, ...) if ((condition)) { PRINT_FORMAT_SDL_WARNING(formatLiteral, __VA_ARGS__) }
