#include "as_titleMenu.h"

#include "application.h"
#include "appState.h"
#include "appStateEvents.h"


AS_TitleMenu asTitleMenu_init(SDL_AppResult *appResult, struct Application *app) {
    AS_TitleMenu menu; SDL_zero(menu);

    *appResult = SDL_APP_CONTINUE;
    return menu;
}

void asTitleMenu_free(AS_TitleMenu *menu) {

}

SDL_AppResult asTitleMenu_onEnter(AS_TitleMenu *menu, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (app->debug) logEnterAppState(AS_TITLE_MENU);

    return appResult;
}

SDL_AppResult asTitleMenu_onExit(AS_TitleMenu *menu, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (app->debug) logExitAppState(AS_TITLE_MENU);

    return appResult;
}

SDL_AppResult asTitleMenu_onUpdate(AS_TitleMenu *menu, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    return appResult;
}

SDL_AppResult asTitleMenu_onRender(AS_TitleMenu *menu, struct Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    return appResult;
}

SDL_AppResult asTitleMenu_onEvent(AS_TitleMenu *menu, SDL_Event *event) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    return appResult;
}
