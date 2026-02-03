#pragma once
#include <SDL3/SDL.h>


#define PRINT_CRITICAL(text) SDL_Log("CRITICAL(%s:%i): %s", __FILE_NAME__, __LINE__, text);
#define PRINT_ERROR(text) SDL_Log("ERROR(%s:%i): %s", __FILE_NAME__, __LINE__, text);
#define PRINT_WARNING(text) SDL_Log("WARNING(%s:%i): %s", __FILE_NAME__, __LINE__, text);

#define PRINT_FORMAT_CRITICAL(formatLiteral, ...) SDL_Log("CRITICAL(%s:%i): " formatLiteral, __FILE_NAME__, __LINE__, __VA_ARGS__);
#define PRINT_FORMAT_ERROR(formatLiteral, ...) SDL_Log("ERROR(%s:%i): " formatLiteral, __FILE_NAME__, __LINE__, __VA_ARGS__);
#define PRINT_FORMAT_WARNING(formatLiteral, ...) SDL_Log("WARNING(%s:%i): " formatLiteral, __FILE_NAME__, __LINE__, __VA_ARGS__);

#define PRINT_SDL_CRITICAL(text) SDL_Log("CRITICAL(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError());
#define PRINT_SDL_ERROR(text) SDL_Log("ERROR(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError());
#define PRINT_SDL_WARNING(text) SDL_Log("WARNING(%s:%i): %s, %s", __FILE_NAME__, __LINE__, text, SDL_GetError());

#define PRINT_FORMAT_SDL_CRITICAL(formatLiteral, ...) SDL_Log("CRITICAL(%s:%i): " formatLiteral ", %s", __FILE_NAME__, __LINE__, __VA_ARGS__, SDL_GetError());
#define PRINT_FORMAT_SDL_ERROR(formatLiteral, ...) SDL_Log("ERROR(%s:%i): " formatLiteral ", %s", __FILE_NAME__, __LINE__, __VA_ARGS__, SDL_GetError());
#define PRINT_FORMAT_SDL_WARNING(formatLiteral, ...) SDL_Log("WARNING(%s:%i): " formatLiteral ", %s", __FILE_NAME__, __LINE__, __VA_ARGS__, SDL_GetError());
