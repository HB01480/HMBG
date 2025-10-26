#pragma once
#include "common.hh"
#include "appState.hh"


class Game_AppState : public AppState {
public:
    Game_AppState(SDL_AppResult *appResult);
    ~Game_AppState();

    SDL_AppResult enter();
    SDL_AppResult exit();

    SDL_AppResult onUpdate(f32 dt);
    SDL_AppResult onRender(SDL_GPUCommandBuffer *gpuCommandBuffer);
    SDL_AppResult onEvent(SDL_Event &event);

private:


};
