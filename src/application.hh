#pragma once
#include "common.hh"


class Application {
public:
    Application(SDL_AppResult *appResult, int argc, char *argv[]);
    ~Application();

    SDL_AppResult onUpdate();
    SDL_AppResult onRender();
    SDL_AppResult onEvent(SDL_Event &event);

private:
    SDL_Window *window;
    SDL_GPUDevice *gpuDevice;

};
