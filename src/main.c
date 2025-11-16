#include "application.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>


typedef struct Program {
    Application app;

} Program;

SDL_AppResult SDLCALL SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    Program *program = SDL_malloc(sizeof(Program));
    SDL_assert(program);
    *appstate = program;

    program->app = application_init(&appResult, argc, argv);

    return appResult;
}

SDL_AppResult SDLCALL SDL_AppIterate(void *appstate) {
    Program *program = (Program *)appstate;
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    appResult = application_update(&program->app);
    appResult = application_render(&program->app);
    return appResult;
}

SDL_AppResult SDLCALL SDL_AppEvent(void *appstate, SDL_Event *event) {
    Program *program = (Program *)appstate;

    return application_processEvents(&program->app, event);
}

void SDLCALL SDL_AppQuit(void *appstate, SDL_AppResult result) {
    Program *program = (Program *)appstate;

    application_free(&program->app);
    SDL_free(program);
}
