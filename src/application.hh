#pragma once
#include "common.hh"

#include "system/clock.hh"


// Their should be only ONE of this.
class Application {
public:
    Application(SDL_AppResult *appResult, int argc, char *argv[]);
    ~Application();

    SDL_AppResult onUpdate();
    SDL_AppResult onRender();
    SDL_AppResult onEvent(SDL_Event &event);

private:
    SDL_AppResult initLibraries();

    void quitLibraries();

    SDL_Window *window;
    SDL_GPUDevice *gpuDevice;
    Clock clock;

};
