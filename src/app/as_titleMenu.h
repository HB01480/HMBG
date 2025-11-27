#pragma once
#include "../common.h"

// Forward declaration from application.h
struct Application;

typedef struct AS_TitleMenu {} AS_TitleMenu;


AS_TitleMenu asTitleMenu_init(SDL_AppResult *result, struct Application *app);
void asTitleMenu_free(AS_TitleMenu *menu);

SDL_AppResult asTitleMenu_onEnter(AS_TitleMenu *menu, struct Application *app);
SDL_AppResult asTitleMenu_onExit(AS_TitleMenu *menu, struct Application *app);

SDL_AppResult asTitleMenu_onUpdate(AS_TitleMenu *menu, struct Application *app);
SDL_AppResult asTitleMenu_onRender(AS_TitleMenu *menu, struct Application *app);
SDL_AppResult asTitleMenu_onEvent(AS_TitleMenu *menu, SDL_Event *event);
