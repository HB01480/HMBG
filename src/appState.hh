#pragma once
#include "common.hh"


enum class AppStateID {
    Null,
    TitleMenu,
    Game,

    Count // For array sizes
};

class AppState {
public:
    AppState();
    virtual ~AppState() = default;

    virtual SDL_AppResult enter() = 0;
    virtual SDL_AppResult exit() = 0;

    virtual SDL_AppResult onUpdate(f32 dt) = 0;
    virtual SDL_AppResult onRender(SDL_GPUCommandBuffer *gpuCommandBuffer) = 0;
    virtual SDL_AppResult onEvent(SDL_Event &event) = 0;

private:
    AppStateID nextAppState;

};
