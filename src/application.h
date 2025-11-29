#pragma once
#include "common.h"
#include "system/clock.h"

#include "app/appState.h"
#include "app/as_titleMenu.h"
#include "app/as_game.h"

#include "render/vertex.h"
#include "render/mesh.h"


typedef struct Application {
    bool debug;

    SDL_Window *window;
    Clock clock;

    AS_TitleMenu asTitleMenu;
    AS_Game asGame;

    AppStateID currentAS;
    AppStateID nextAS;

    SDL_GPUDevice *gpu;
    SDL_GPUBuffer *vertexBuffer;
    SDL_GPUBuffer *indexBuffer;
    SDL_GPUBuffer *meshBuffer;
    SDL_GPUTransferBuffer *transferBuffer;
    SDL_GPUShader *vertexShader;
    SDL_GPUShader *fragmentShader;
    SDL_GPUGraphicsPipeline *graphicsPipeline;

    RenderMesh testMesh;

} Application;


Application application_init(SDL_AppResult *outResult, int argumentCount, char *arguments[]);
void application_free(Application *app);

SDL_AppResult application_onUpdate(Application *app);
SDL_AppResult application_onRender(Application *app);
SDL_AppResult application_onEvent(Application *app, SDL_Event *event);
