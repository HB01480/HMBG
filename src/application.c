#include "application.h"


static bool isApplicationConstructed = false;

SDL_AppResult application_init(Application *outApp, int argumentCount, char *arguments[]) {
    HANDLE_ERROR(isApplicationConstructed, "The application was already successfully constructed", handleAppFailure);
    HANDLE_ERROR(!outApp, "Parameter 'outApp' is NULL", handleAppFailure)

    Application *app = SDL_malloc(sizeof(Application));
    HANDLE_ERROR(!app, "Failed to allocate memory for the application", handleAppFailure)
    SDL_zerop(app);

    app->debug = false;
    if (argumentCount > 1) {
        if (SDL_strcmp(arguments[1], "debug")) {
            app->debug = true;
        }
    }

    SDL_WindowFlags windowFlags = SDL_WINDOW_HIDDEN;
    app->window = SDL_CreateWindow("Highly Moddable Block Game", 1024, 512, windowFlags);
    if (!app->window) {
        SDL_Log("Failed to construct the window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    HANDLE_SDL_ERROR(!app->window, "Failed to construct the window", handleAppFailure);

    app->gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, app->debug, NULL);
    HANDLE_SDL_ERROR(!app->gpu, "Failed to construct the gpu device", handleAppFailure);
    HANDLE_SDL_ERROR(!SDL_ClaimWindowForGPUDevice(app->gpu, app->window), "Failed to claim the window for the gpu device", handleAppFailure);

    app->clock = clock_init();

    app->keyState = SDL_GetKeyboardState(NULL);
    app->mouseState = mouseState_init();

    {
        const RenderVertex vertices[] = {
            {.position = {-0.5f,  0.5f, 0.0f}, .texCoord = {0.0f, 1.0f}}, // tl
            {.position = { 0.5f,  0.5f, 0.0f}, .texCoord = {1.0f, 1.0f}}, // tr
            {.position = { 0.5f, -0.5f, 0.0f}, .texCoord = {1.0f, 0.0f}}, // br
            {.position = {-0.5f, -0.5f, 0.0f}, .texCoord = {0.0f, 0.0f}}  // bl
        };
        const u32 indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        app->testMesh = renderMesh_init(vertices, sizeof(vertices), indices, sizeof(indices));
    }

    app->testMesh_modelMatrix = glms_translate_make(glms_vec3_make((f32 []){ 0.0f,  0.0f, -1.0f}));
    app->renderCamera = renderCamera_init(
        glms_vec3_make((f32 []){ 0.0f,  0.0f,  0.0f}),
        glms_vec3_make((f32 []){ 0.0f,  1.0f,  0.0f}),
        0.0f, 0.0f,
        5.0f, 2.5f
    );

    app->vertexBuffer = SDL_CreateGPUBuffer(app->gpu, &(SDL_GPUBufferCreateInfo){
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = app->testMesh.verticesArraySize,
        .props = 0
    });
    HANDLE_SDL_ERROR(!app->vertexBuffer, "Failed to construct the vertex buffer", handleAppFailure)

    app->indexBuffer = SDL_CreateGPUBuffer(app->gpu, &(SDL_GPUBufferCreateInfo){
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = app->testMesh.indicesArraySize,
        .props = 0
    });
    HANDLE_SDL_ERROR(!app->indexBuffer, "Failed to construct the index buffer", handleAppFailure)

    app->transferBuffer = SDL_CreateGPUTransferBuffer(app->gpu, &(SDL_GPUTransferBufferCreateInfo){
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = app->testMesh.verticesArraySize + app->testMesh.indicesArraySize,
        .props = 0
    });
    HANDLE_SDL_ERROR(!app->transferBuffer, "Failed to construct the transfer buffer", handleAppFailure)

    SDL_Storage *contentStorage = SDL_OpenTitleStorage(NULL, 0);
    HANDLE_SDL_ERROR(!contentStorage, "Failed to open content storage", handleAppFailure)

    while (!SDL_StorageReady(contentStorage)) {
        SDL_Log("Waiting for content storage to be ready");
        SDL_Delay(1);
    }

    {
        void *transferBufferPtr = SDL_MapGPUTransferBuffer(app->gpu, app->transferBuffer, false);
        void *verticesPtr = transferBufferPtr;
        void *indicesPtr = transferBufferPtr + app->testMesh.verticesArraySize;

        SDL_memcpy(verticesPtr, app->testMesh.vertices, app->testMesh.verticesArraySize);
        SDL_memcpy(indicesPtr, app->testMesh.indices, app->testMesh.indicesArraySize);

        SDL_UnmapGPUTransferBuffer(app->gpu, transferBufferPtr);
    }

    {
        const char *vertexShaderPath = "res/shaders/basic.vert.spv";
        const char *fragmentShaderPath = "res/shaders/basic.frag.spv";
        usize vertexShaderCodeSize = 0, fragmentShaderCodeSize = 0;
        void *vertexShaderCode = NULL;
        void *fragmentShaderCode = NULL;

        vertexShaderCode = SDLext_LoadStorageFile(&vertexShaderCodeSize, contentStorage, vertexShaderPath);
        HANDLE_SDL_ERROR(!vertexShaderCode, "Failed to load vertex shader code from content storage", handleAppFailure)

        fragmentShaderCode = SDLext_LoadStorageFile(&fragmentShaderCodeSize, contentStorage, fragmentShaderPath);
        HANDLE_SDL_ERROR(!fragmentShaderCode, "Failed to load fragment shader code from content storage", handleAppFailure)

        SDL_GPUShader *vertexShader = SDL_CreateGPUShader(app->gpu, &(SDL_GPUShaderCreateInfo){
            .code = vertexShaderCode,
            .code_size = vertexShaderCodeSize,
            .entrypoint = "main",
            .format = SDL_GPU_SHADERFORMAT_SPIRV,
            .stage = SDL_GPU_SHADERSTAGE_VERTEX,
            .num_samplers = 0,
            .num_storage_buffers = 0,
            .num_storage_textures = 0,
            .num_uniform_buffers = 1,
            .props = 0
        });
        HANDLE_SDL_ERROR(!vertexShader, "Failed to construct a vertex shader", handleAppFailure)

        SDL_GPUShader *fragmentShader = SDL_CreateGPUShader(app->gpu, &(SDL_GPUShaderCreateInfo){
            .code = fragmentShaderCode,
            .code_size = fragmentShaderCodeSize,
            .entrypoint = "main",
            .format = SDL_GPU_SHADERFORMAT_SPIRV,
            .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
            .num_samplers = 0,
            .num_storage_buffers = 0,
            .num_storage_textures = 0,
            .num_uniform_buffers = 0,
            .props = 0
        });
        HANDLE_SDL_ERROR(!fragmentShader, "Failed to construct a fragment shader", handleAppFailure)

        SDL_free(vertexShaderCode);
        SDL_free(fragmentShaderCode);

        app->graphicsPipeline = SDL_CreateGPUGraphicsPipeline(app->gpu, &(SDL_GPUGraphicsPipelineCreateInfo){
            .vertex_shader = vertexShader,
            .fragment_shader = fragmentShader,
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,

            .target_info = {
                .has_depth_stencil_target = false,
                .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_INVALID,
                .num_color_targets = 1,
                .color_target_descriptions = (SDL_GPUColorTargetDescription []){
                    {
                        .blend_state = {
                            .enable_blend = true,
                            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
                            .color_blend_op = SDL_GPU_BLENDOP_ADD,
                            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA
                        },
                        .format = SDL_GetGPUSwapchainTextureFormat(app->gpu, app->window)
                    }
                }
            },
            .vertex_input_state = {
                .num_vertex_buffers = 1,
                .vertex_buffer_descriptions = (SDL_GPUVertexBufferDescription []){
                    {
                        .slot = 0,
                        .instance_step_rate = 1,
                        .pitch = sizeof(RenderVertex),
                        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX
                    }
                },
                .num_vertex_attributes = 1,
                .vertex_attributes = (SDL_GPUVertexAttribute []){
                    {
                        .buffer_slot = 0,
                        .location = 0,
                        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                        .offset = 0,
                    }
                }
            },

            .depth_stencil_state = {},
            .multisample_state = {},
            .rasterizer_state = {},

            .props = 0
        });
        HANDLE_SDL_ERROR(!app->graphicsPipeline, "Failed to construct the graphics pipeline", handleAppFailure)

        SDL_ReleaseGPUShader(app->gpu, vertexShader);
        SDL_ReleaseGPUShader(app->gpu, fragmentShader);
    }

    application_enableRelativeModeMousing(app);

    SDL_ShowWindow(app->window);
    *outApp = *app;
    isApplicationConstructed = true;
    return SDL_APP_CONTINUE;

    handleAppFailure:
    return SDL_APP_FAILURE;
}

void application_free(Application *app) {
    application_disableRelativeModeMousing(app);

    renderMesh_free(&app->testMesh);

    SDL_ReleaseGPUBuffer(app->gpu, app->vertexBuffer);
    SDL_ReleaseGPUBuffer(app->gpu, app->indexBuffer);
    SDL_ReleaseGPUTransferBuffer(app->gpu, app->transferBuffer);
    SDL_ReleaseGPUGraphicsPipeline(app->gpu, app->graphicsPipeline);

    SDL_DestroyGPUDevice(app->gpu);
    SDL_DestroyWindow(app->window);

    SDL_free(app);
    isApplicationConstructed = false;
}

SDL_AppResult application_onUpdate(Application *app) {
    if (app->keyState[SDL_SCANCODE_W]) renderCamera_moveForward(&app->renderCamera, app->clock.dt);
    if (app->keyState[SDL_SCANCODE_S]) renderCamera_moveBackward(&app->renderCamera, app->clock.dt);
    if (app->keyState[SDL_SCANCODE_A]) renderCamera_moveLeftward(&app->renderCamera, app->clock.dt);
    if (app->keyState[SDL_SCANCODE_D]) renderCamera_moveRightward(&app->renderCamera, app->clock.dt);

    SDL_Log("%f, %f", app->renderCamera.position.x, app->renderCamera.position.y);

    mouseState_update(&app->mouseState);
    clock_tick(&app->clock);
    return SDL_APP_CONTINUE;
}

SDL_AppResult application_onRender(Application *app) {
    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(app->gpu);

    SDL_GPUTexture *windowTexture = NULL;
    u32 windowWidth = 0, windowHeight = 0;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, app->window, &windowTexture, &windowWidth, &windowHeight)) {
        SDL_Log("Failed to acquire gpu swapchain texture: %s", SDL_GetError());
        SDL_SubmitGPUCommandBuffer(commandBuffer);
        return SDL_APP_FAILURE;
    }

    if (!windowTexture) {
        goto renderCleanUp;
    }

    SDL_GPUColorTargetInfo colorTargetInfos[1]; SDL_zeroa(colorTargetInfos);
    colorTargetInfos[0].texture = windowTexture;
    colorTargetInfos[0].clear_color = (SDL_FColor){0.0f, 0.0f, 0.0f, 1.0f};
    colorTargetInfos[0].load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfos[0].store_op = SDL_GPU_STOREOP_STORE; 

    SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);

    SDL_UploadToGPUBuffer(
        copyPass, &(SDL_GPUTransferBufferLocation){
            .transfer_buffer = app->transferBuffer,
            .offset = 0
        },
        &(SDL_GPUBufferRegion){
            .buffer = app->vertexBuffer,
            .size = app->testMesh.verticesArraySize,
            .offset = 0
        }, false
    );
    SDL_UploadToGPUBuffer(
        copyPass, &(SDL_GPUTransferBufferLocation){
            .transfer_buffer = app->transferBuffer,
            .offset = app->testMesh.verticesArraySize
        },
        &(SDL_GPUBufferRegion){
            .buffer = app->indexBuffer,
            .size = app->testMesh.indicesArraySize,
            .offset = 0
        }, false
    );

    SDL_EndGPUCopyPass(copyPass);
    SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, colorTargetInfos, SDL_arraysize(colorTargetInfos), NULL);

    SDL_BindGPUGraphicsPipeline(renderPass, app->graphicsPipeline);
    SDL_BindGPUVertexBuffers(
        renderPass, 0,
        (SDL_GPUBufferBinding []){
            {
                .buffer = app->vertexBuffer,
                .offset = 0
            }
        }, 1
    );

    SDL_BindGPUIndexBuffer(
        renderPass,
        &(SDL_GPUBufferBinding){
            .buffer = app->indexBuffer,
            .offset = 0
        }, SDL_GPU_INDEXELEMENTSIZE_32BIT
    );

    BasicUBO ubo; SDL_zero(ubo);
    ubo.model = app->testMesh_modelMatrix;
    ubo.view = renderCamera_calculateViewMatrix(&app->renderCamera);
    ubo.projection = calculatePerspectiveMatrixFromWindow(app->window);
    ubo.timeSeconds = SDL_GetTicks() / 1000.0f;

    SDL_PushGPUVertexUniformData(commandBuffer, 0, &ubo, sizeof(ubo));

    SDL_DrawGPUIndexedPrimitives(renderPass, app->testMesh.indicesArraySize / sizeof(u32), 1, 0, 0, 0);

    SDL_EndGPURenderPass(renderPass);

    renderCleanUp:
    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult application_onEvent(Application *app, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        vec2s delta = glms_vec2_zero();
        delta.x = event->motion.xrel;
        delta.y = event->motion.yrel;

        renderCamera_pan(&app->renderCamera, delta, app->clock.dt);
    }

    return SDL_APP_CONTINUE;
}

void application_enableRelativeModeMousing(Application *app) {
    SDL_SetWindowRelativeMouseMode(app->window, true);
}

void application_disableRelativeModeMousing(Application *app) {
    SDL_SetWindowRelativeMouseMode(app->window, false);
}

mat4s calculatePerspectiveMatrixFromWindow(SDL_Window *window) {
    ivec2s size = glms_ivec2_zero(); SDL_GetWindowSizeInPixels(window, &size.x, &size.y);    
    f32 aspectRatio = (f32)size.x/(f32)size.y;

    return glms_perspective(45.0f * DEGREES_TO_RADIANS, aspectRatio, 0.1f, 100.0f);
}
