#pragma once
#include "common.hh"

#include "system/clock.hh"
#include "appState.hh"
#include "game_appState.hh"
#include "titleMenu_appState.hh"


#define APP_STATE_EVENT_CODE_CHANGE_TO 0

// Their should be only ONE of this.
class Application {
public:
    Application(SDL_AppResult *appResult, int argc, char *argv[]);
    ~Application();

    SDL_AppResult onIterate();
    SDL_AppResult onEvent(SDL_Event &event);

    static u32 getAppStateEventType();
    static void sendASEventChangeTo(AppStateID id);
    static AppStateID receiveASEventChangeTo(SDL_Event &event);

private:
    SDL_AppResult onUpdate();
    SDL_AppResult onRender();

    SDL_AppResult initLibraries();

    void quitLibraries();

    SDL_Window *window;
    SDL_GPUDevice *gpu;
    Clock clock;

    Game_AppState *gameAS;
    TitleMenu_AppState *titleMenuAS;

    AppState *currentAS;
    AppState *nextAS;

    SDL_GPUShader *vertexShader;
    SDL_GPUShader *fragmentShader;

    SDL_GPUBuffer *vertexBuffer;
    SDL_GPUTransferBuffer *transferBuffer;

    SDL_GPUGraphicsPipeline *graphicsPipeline;

};
