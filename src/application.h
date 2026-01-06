#pragma once
#include "common.h"
#include "app/clock.h"
#include "app/keyState.h"
#include "app/mouseState.h"

#include "render/vertex.h"
#include "render/mesh.h"
#include "render/basicUniformBuffer.h"
#include "render/camera.h"


typedef struct Application {
    SDL_Window *window;
    SDL_GPUDevice *gpu;
    Clock clock;
    bool debug;

    KeyState keyState;
    MouseState mouseState;

    RenderMesh testMesh;
    mat4s testMesh_modelMatrix;
    RenderCamera renderCamera;

    SDL_Surface *testImage;
    SDL_GPUTexture *testTexture;
    SDL_GPUSampler *testTextureSampler;

    SDL_GPUGraphicsPipeline *graphicsPipeline;
    SDL_GPUBuffer *vertexBuffer;
    SDL_GPUBuffer *indexBuffer;
    SDL_GPUTransferBuffer *transferBuffer;
    SDL_GPUTransferBuffer *textureTransferBuffer;
    

} Application;


SDL_AppResult application_init(Application *outApp, int argumentCount, char *arguments[]);
void application_free(Application *app);

SDL_AppResult application_onUpdate(Application *app);
SDL_AppResult application_onRender(Application *app);
SDL_AppResult application_onEvent(Application *app, SDL_Event *event);

void application_enableRelativeModeMousing(Application *app);
void application_disableRelativeModeMousing(Application *app);

mat4s calculatePerspectiveMatrixFromWindow(SDL_Window *window);
