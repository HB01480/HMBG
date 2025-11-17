#pragma once
#include "../common.h"
#include "../system/clock.h"

#include "../render/vertex.h"
#include "../render/mesh.h"


typedef struct Application {
    SDL_Window *window;
    Clock clock;

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

SDL_AppResult application_update(Application *app);
SDL_AppResult application_render(Application *app);
SDL_AppResult application_processEvents(Application *app, SDL_Event *event);
