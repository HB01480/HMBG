#pragma once
#include "../common.h"

// Forward declaration from application.h
struct Application;

typedef struct AS_Game {} AS_Game;


AS_Game asGame_init(SDL_AppResult *appResult, struct Application *app);
void asGame_free(AS_Game *game);

SDL_AppResult asGame_onEnter(AS_Game *game, struct Application *app);
SDL_AppResult asGame_onExit(AS_Game *game, struct Application *app);

SDL_AppResult asGame_onUpdate(AS_Game *game, struct Application *app);
SDL_AppResult asGame_onRender(AS_Game *game, struct Application *app);
SDL_AppResult asGame_onEvent(AS_Game *game, SDL_Event *event);
