#pragma once
#include "../../common.h"


typedef enum AppStateID {
    AS_NULL,
    AS_TITLE_MENU,
    AS_GAME,

    AS_COUNT
} AppStateID;


const static char *appStateNames[] = {
    [AS_NULL] = "NULL",
    [AS_TITLE_MENU] = "Title Menu",
    [AS_GAME] = "Game",

    [AS_COUNT] = "NULL Count"
};


typedef SDL_AppResult (*AppStateOnEnter)(void *stateData);
typedef SDL_AppResult (*AppStateOnExit)(void *stateData);

typedef SDL_AppResult (*AppStateOnUpdate)(void *stateData);
typedef SDL_AppResult (*AppStateOnRender)(void *stateData);
typedef SDL_AppResult (*AppStateOnEvent)(void *stateData, SDL_Event *event);
