#pragma once
#include "common.h"
#include "system/clock.h"

#include "app/appState.h"
#include "app/as_titleMenu.h"
#include "app/as_game.h"
#include "app/mouseState.h"
#include "app/keyState.h"

#include "render/vertex.h"
#include "render/mesh.h"
#include "render/basicUniformBuffer.h"
#include "render/camera.h"


typedef struct Application {
    bool debug;

    SDL_Window *window;
    Clock clock;

    MouseState mouseState;
    KeyState keyState;

    AS_TitleMenu asTitleMenu;
    AS_Game asGame;

    AppStateID currentAS;
    AppStateID nextAS;

    RenderCamera camera;
    RenderMesh testMesh;

    BasicUBO basicUBO;

    SDL_GPUDevice *gpu;
    SDL_GPUBuffer *vertexBuffer;
    SDL_GPUBuffer *indexBuffer;
    SDL_GPUBuffer *meshBuffer;
    SDL_GPUTransferBuffer *transferBuffer;
    SDL_GPUShader *vertexShader;
    SDL_GPUShader *fragmentShader;
    SDL_GPUGraphicsPipeline *graphicsPipeline;

} Application;


Application application_init(SDL_AppResult *outResult, int argumentCount, char *arguments[]);
void application_free(Application *app);

SDL_AppResult application_onUpdate(Application *app);
SDL_AppResult application_onRender(Application *app);
SDL_AppResult application_onEvent(Application *app, SDL_Event *event);
