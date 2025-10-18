#include "application.hh"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>


SDL_AppResult SDLCALL SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    Application *app = new Application{&appResult, argc, argv};
    *appstate = app;

    return appResult;
}

SDL_AppResult SDLCALL SDL_AppIterate(void *appstate) {
    Application *app = (Application *)appstate;

    SDL_AppResult appResult = SDL_APP_CONTINUE;
    appResult = app->onUpdate();
    appResult = app->onRender();

    return appResult;
}

SDL_AppResult SDLCALL SDL_AppEvent(void *appstate, SDL_Event *event) {
    Application *app = (Application *)appstate;

    return app->onEvent(*event);
}

void SDLCALL SDL_AppQuit(void *appstate, SDL_AppResult result) {
    // Application *app = (Application *)appstate;
    // app's destructor should be called around here
}
