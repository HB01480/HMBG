#include "titleMenu_appState.hh"


TitleMenu_AppState::TitleMenu_AppState(SDL_AppResult *appResult)
: AppState()
{
    
}

TitleMenu_AppState::~TitleMenu_AppState() {

}

SDL_AppResult TitleMenu_AppState::enter() {
    SDL_Log("Entered TitleMenu");

    return SDL_APP_CONTINUE;
}

SDL_AppResult TitleMenu_AppState::exit() {
    SDL_Log("Exited TitleMenu");

    return SDL_APP_CONTINUE;
}

SDL_AppResult TitleMenu_AppState::onUpdate(f32 dt) {


    return SDL_APP_CONTINUE;
}

SDL_AppResult TitleMenu_AppState::onRender(SDL_GPUCommandBuffer *gpuCommandBuffer) {


    return SDL_APP_CONTINUE;
}

SDL_AppResult TitleMenu_AppState::onEvent(SDL_Event &event) {


    return SDL_APP_CONTINUE;
}
