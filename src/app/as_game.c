#include "as_game.h"

#include "application.h"
#include "appState.h"
#include "appStateEvents.h"


AS_Game asGame_init(SDL_AppResult *appResult, struct Application *app) {
    AS_Game game; SDL_zero(game);

    *appResult = SDL_APP_CONTINUE;
    return game;
}

void asGame_free(AS_Game *game) {

}

SDL_AppResult asGame_onEnter(AS_Game *game, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (app->debug) logEnterAppState(AS_GAME);

    return appResult;
}

SDL_AppResult asGame_onExit(AS_Game *game, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (app->debug) logExitAppState(AS_GAME);

    return appResult;
}

SDL_AppResult asGame_onUpdate(AS_Game *game, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    return appResult;
}

SDL_AppResult asGame_onRender(AS_Game *game, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    return appResult;
}

SDL_AppResult asGame_onEvent(AS_Game *game, SDL_Event *event) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    return appResult;
}
