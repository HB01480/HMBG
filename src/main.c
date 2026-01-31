#include "application.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>


SDL_AppResult SDLCALL SDL_AppInit(void **appstate, int argc, char *argv[]) {
    Application *app = SDL_malloc(sizeof(Application));
    if (!app) {
        SDL_Log("Failed to allocate memory for the application struct:\n%s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = app;
    return application_init(app, argc, argv);
}

SDL_AppResult SDLCALL SDL_AppIterate(void *appstate) {
    Application *app = (Application *)appstate;
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    appResult = application_onUpdate(app);
    appResult = application_onRender(app);
    return appResult;
}

SDL_AppResult SDLCALL SDL_AppEvent(void *appstate, SDL_Event *event) {
    Application *app = (Application *)appstate;

    return application_onEvent(app, event);
}

void SDLCALL SDL_AppQuit(void *appstate, SDL_AppResult result) {
    Application *app = (Application *)appstate;
    if (!app) {
        return;
    }

    application_free(app);
}
