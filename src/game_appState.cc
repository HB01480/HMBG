#include "game_appState.hh"


Game_AppState::Game_AppState(SDL_AppResult *appResult) {

}

Game_AppState::~Game_AppState() {

}

SDL_AppResult Game_AppState::enter() {
    SDL_Log("Entered Game");

    return SDL_APP_CONTINUE;
}

SDL_AppResult Game_AppState::exit() {
    SDL_Log("Exit Game");

    return SDL_APP_CONTINUE;
}

SDL_AppResult Game_AppState::onUpdate(f32 dt) {


    return SDL_APP_CONTINUE;
}

SDL_AppResult Game_AppState::onRender(SDL_GPUCommandBuffer *gpuCommandBuffer) {


    return SDL_APP_CONTINUE;
}

SDL_AppResult Game_AppState::onEvent(SDL_Event &event) {


    return SDL_APP_CONTINUE;
}
