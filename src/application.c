#include "application.h"

#include "app/appStateEvents.h"


SDL_AppResult application_initSDL();
void application_quitSDL();


Application application_init(SDL_AppResult *outResult, int argumentCount, char *arguments[]) {
    Application app; SDL_zero(app);

    app.debug = false;
    if (argumentCount > 1) {
        if (SDL_strcmp(arguments[1], "debug") == 0) {
            app.debug = true;
        }
    }

    const char *appTitle = "Highly Moddable Block Game";
    const s32 appWidth = 1024;
    const s32 appHeight = 512;

    SDL_SetAppMetadata(appTitle, NULL, "com.hb01480.hmbg");
    application_initSDL();

    SDL_WindowFlags windowFlags = SDL_WINDOW_HIDDEN;
    app.window = SDL_CreateWindow(appTitle, appWidth, appHeight, windowFlags);
    if (!app.window) {
        SDL_Log("Failed to create window:\n%s", SDL_GetError());
        *outResult = SDL_APP_FAILURE;
    }
    app.clock = (Clock){};

    application_enableRelativeMouseMode(&app);

    SDL_zero(app.mouseState);
    // Since it's a pointer to a internal SDL array,
    // it gets automatically updated after all SDL events are processed.
    app.keyState = SDL_GetKeyboardState(NULL);

    app.gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, app.debug, NULL);
    if (!app.gpu) {
        SDL_Log("Failed to create GPU Device:\n%s", SDL_GetError());
        *outResult = SDL_APP_FAILURE;
    }

    if (!SDL_ClaimWindowForGPUDevice(app.gpu, app.window)) {
        SDL_Log("Failed to claim the application's window for the its gpu device:\n%s", SDL_GetError());
        *outResult = SDL_APP_FAILURE;
    }

    SDL_Log("--------Application-Information-----------");
    SDL_Log("Platform: %s", SDL_GetPlatform());
    SDL_Log("GPU Backend: %s", SDL_GetGPUDeviceDriver(app.gpu));
    SDL_Log("SDL Version: %i.%i.%i", SDL_VERSIONNUM_MAJOR(SDL_GetVersion()), SDL_VERSIONNUM_MINOR(SDL_GetVersion()), SDL_VERSIONNUM_MICRO(SDL_GetVersion())); 
    SDL_Log("------------------------------------------");

    app.asTitleMenu = asTitleMenu_init(outResult, &app);
    app.asGame = asGame_init(outResult, &app);

    app.currentAS = AS_TITLE_MENU;
    app.nextAS = AS_NULL;

    if (app.currentAS == AS_TITLE_MENU) *outResult = asTitleMenu_onEnter(&app.asTitleMenu, &app);
    if (app.currentAS == AS_GAME) *outResult = asGame_onEnter(&app.asGame, &app);

    {
        const RenderVertex vertices[] = {
            {{ 0.5f,  0.5f,  0.0f}},
            {{ 0.5f, -0.5f,  0.0f}},
            {{-0.5f, -0.5f,  0.0f}},
            {{-0.5f,  0.5f,  0.0f}}
        };
        const u32 indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        app.testMesh = renderMesh_init(vertices, sizeof(vertices), indices, sizeof(indices));
    }

    SDL_GPUBufferCreateInfo vertexBufferInfo = {};
    vertexBufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertexBufferInfo.size = app.testMesh.verticesArraySize;
    app.vertexBuffer = SDL_CreateGPUBuffer(app.gpu, &vertexBufferInfo);

    SDL_GPUBufferCreateInfo indexBufferInfo = {};
    indexBufferInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    indexBufferInfo.size = app.testMesh.indicesArraySize;
    app.indexBuffer = SDL_CreateGPUBuffer(app.gpu, &indexBufferInfo);

    SDL_GPUTransferBufferCreateInfo transferBufferInfo = {};
    transferBufferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferBufferInfo.size = app.testMesh.verticesArraySize + app.testMesh.indicesArraySize;
    app.transferBuffer = SDL_CreateGPUTransferBuffer(app.gpu, &transferBufferInfo);

    void *mappedBaseTransferBuffer = SDL_MapGPUTransferBuffer(app.gpu, app.transferBuffer, false);
    RenderVertex *verticesLocation = mappedBaseTransferBuffer;
    u32 *indicesLocation = mappedBaseTransferBuffer + app.testMesh.verticesArraySize;

    SDL_memcpy(verticesLocation, (void *)app.testMesh.vertices, app.testMesh.verticesArraySize);
    SDL_memcpy(indicesLocation, (void *)app.testMesh.indices, app.testMesh.indicesArraySize);

    SDL_UnmapGPUTransferBuffer(app.gpu, app.transferBuffer);

    SDL_Storage *gameStorage = SDL_OpenTitleStorage(NULL, 0);
    if (!gameStorage) {
        SDL_Log("Failed to open storage for game data:\n%s", SDL_GetError());
        *outResult = SDL_APP_FAILURE;
    }
    while (!SDL_StorageReady(gameStorage)) {
        SDL_Log("Waiting for game storage to be ready");
        SDL_Delay(1);
    }

    const char *vertexShaderPath = "res/shaders/basic.vert.spv";
    const char *fragmentShaderPath = "res/shaders/basic.frag.spv";

    u64 vertexShaderCodeSize = 0;
    u64 fragmentShaderCodeSize = 0;
    if (!SDL_GetStorageFileSize(gameStorage, vertexShaderPath, &vertexShaderCodeSize)) {
        SDL_Log("Can't get the file size of '%s'", vertexShaderPath);
        *outResult = SDL_APP_FAILURE;
    }
    if (!SDL_GetStorageFileSize(gameStorage, fragmentShaderPath, &fragmentShaderCodeSize)) {
        SDL_Log("Can't get the file size of '%s'", fragmentShaderPath);
        *outResult = SDL_APP_FAILURE;
    }
    u8 *vertexShaderCode = SDL_malloc(vertexShaderCodeSize);
    u8 *fragmentShaderCode = SDL_malloc(fragmentShaderCodeSize);
    SDL_assert(vertexShaderCode);
    SDL_assert(fragmentShaderCode);

    if (!SDL_ReadStorageFile(gameStorage, vertexShaderPath, (void *)vertexShaderCode, vertexShaderCodeSize)) {
        SDL_Log("Can't read the file of '%s'", vertexShaderPath);
        *outResult = SDL_APP_FAILURE;
    }
    if (!SDL_ReadStorageFile(gameStorage, fragmentShaderPath, (void *)fragmentShaderCode, fragmentShaderCodeSize)) {
        SDL_Log("Can't read the file of '%s'", fragmentShaderPath);
        *outResult = SDL_APP_FAILURE;
    }

    SDL_CloseStorage(gameStorage);

    SDL_GPUShaderCreateInfo vertexShaderInfo = {};
    vertexShaderInfo.code_size = vertexShaderCodeSize;
    vertexShaderInfo.code = vertexShaderCode;
    vertexShaderInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    vertexShaderInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
    vertexShaderInfo.num_samplers = 0;
    vertexShaderInfo.num_storage_buffers = 0;
    vertexShaderInfo.num_storage_textures = 0;
    vertexShaderInfo.num_uniform_buffers = 1;

    app.vertexShader = SDL_CreateGPUShader(app.gpu, &vertexShaderInfo);

    SDL_GPUShaderCreateInfo fragmentShaderInfo = {};
    fragmentShaderInfo.code_size = fragmentShaderCodeSize;
    fragmentShaderInfo.code = fragmentShaderCode;
    fragmentShaderInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    fragmentShaderInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
    fragmentShaderInfo.num_samplers = 0;
    fragmentShaderInfo.num_storage_buffers = 0;
    fragmentShaderInfo.num_storage_textures = 0;
    fragmentShaderInfo.num_uniform_buffers = 1;

    app.fragmentShader = SDL_CreateGPUShader(app.gpu, &fragmentShaderInfo);

    SDL_free(vertexShaderCode);
    SDL_free(fragmentShaderCode);

    SDL_GPUGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
    graphicsPipelineInfo.vertex_shader = app.vertexShader;
    graphicsPipelineInfo.fragment_shader = app.fragmentShader;
    graphicsPipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    SDL_GPUVertexBufferDescription vertexBufferDescriptions[1]; SDL_zeroa(vertexBufferDescriptions);
    vertexBufferDescriptions[0].slot = 0;
    vertexBufferDescriptions[0].pitch = sizeof(RenderVertex);
    vertexBufferDescriptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertexBufferDescriptions[0].instance_step_rate = 0;

    graphicsPipelineInfo.vertex_input_state.num_vertex_buffers = sizeof(vertexBufferDescriptions) / sizeof(SDL_GPUVertexBufferDescription);
    graphicsPipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDescriptions;

    SDL_GPUVertexAttribute vertexAttributeDescriptions[1]; SDL_zeroa(vertexAttributeDescriptions);
    vertexAttributeDescriptions[0].buffer_slot = 0;
    vertexAttributeDescriptions[0].location = 0;
    vertexAttributeDescriptions[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertexAttributeDescriptions[0].offset = offsetof(RenderVertex, position);

    graphicsPipelineInfo.vertex_input_state.num_vertex_attributes = sizeof(vertexAttributeDescriptions) / sizeof(SDL_GPUVertexAttribute);
    graphicsPipelineInfo.vertex_input_state.vertex_attributes = vertexAttributeDescriptions;

    SDL_GPUColorTargetDescription colorTargetDescriptions[1]; SDL_zeroa(colorTargetDescriptions);
    colorTargetDescriptions[0].blend_state.enable_blend = true;
    colorTargetDescriptions[0].blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
    colorTargetDescriptions[0].blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    colorTargetDescriptions[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    colorTargetDescriptions[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    colorTargetDescriptions[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    colorTargetDescriptions[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    colorTargetDescriptions[0].format = SDL_GetGPUSwapchainTextureFormat(app.gpu, app.window);

    graphicsPipelineInfo.target_info.num_color_targets = sizeof(colorTargetDescriptions) / sizeof(SDL_GPUColorTargetDescription);
    graphicsPipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

    app.graphicsPipeline = SDL_CreateGPUGraphicsPipeline(app.gpu, &graphicsPipelineInfo);

    app.camera = renderCamera_init(
        (vec3s){{0.0f, 0.0f, 0.0f}},
        (vec3s){{0.0f, 1.0f, 0.0f}},
        0.0f, -90.0f,
        5.0f, 2.5f
    );

    app.basicUBO.time = SDL_GetTicks() / 1000.0f;
    app.basicUBO.model = glms_translate_make(glms_vec3_make((f32 []){0.0f, 0.0f, -1.0f}));
    app.basicUBO.view = glms_mat4_identity();
    app.basicUBO.projection = glms_mat4_identity();

    SDL_ShowWindow(app.window);

    return app;
}

void application_free(Application *app) {
    if (app->currentAS == AS_TITLE_MENU) asTitleMenu_onExit(&app->asTitleMenu, app);
    if (app->currentAS == AS_GAME) asGame_onExit(&app->asGame, app);

    application_disableRelativeMouseMode(app);

    asGame_free(&app->asGame);
    asTitleMenu_free(&app->asTitleMenu);

    renderMesh_free(&app->testMesh);

    SDL_ReleaseGPUGraphicsPipeline(app->gpu, app->graphicsPipeline);

    SDL_ReleaseGPUShader(app->gpu, app->vertexShader);
    SDL_ReleaseGPUShader(app->gpu, app->fragmentShader);

    SDL_ReleaseGPUTransferBuffer(app->gpu, app->transferBuffer);
    SDL_ReleaseGPUBuffer(app->gpu, app->indexBuffer);
    SDL_ReleaseGPUBuffer(app->gpu, app->vertexBuffer);

    SDL_DestroyGPUDevice(app->gpu);

    SDL_DestroyWindow(app->window);

    application_quitSDL();
}

SDL_AppResult application_onUpdate(Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (app->currentAS == AS_TITLE_MENU) asTitleMenu_onUpdate(&app->asTitleMenu, app);
    if (app->currentAS == AS_GAME) asGame_onUpdate(&app->asGame, app);
    
    if (app->nextAS != AS_NULL) {
        if (app->currentAS == AS_TITLE_MENU) asTitleMenu_onExit(&app->asTitleMenu, app);
        if (app->currentAS == AS_GAME) asGame_onExit(&app->asGame, app);
        if (app->nextAS == AS_TITLE_MENU) asTitleMenu_onEnter(&app->asTitleMenu, app);
        if (app->nextAS == AS_GAME) asGame_onEnter(&app->asGame, app);

        app->currentAS = app->nextAS;
        app->nextAS = AS_NULL;
    }

    if (app->keyState[SDL_SCANCODE_W])
        renderCamera_moveForward(&app->camera, app->clock.dt);
    if (app->keyState[SDL_SCANCODE_S])
        renderCamera_moveBackward(&app->camera, app->clock.dt);
    if (app->keyState[SDL_SCANCODE_A])
        renderCamera_moveLeftward(&app->camera, app->clock.dt);
    if (app->keyState[SDL_SCANCODE_D])
        renderCamera_moveRightward(&app->camera, app->clock.dt);

    app->basicUBO.model = glms_rotate(app->basicUBO.model, 45.0f * DEGREES_TO_RADIANS * app->clock.dt, (vec3s){{0.0f, 0.0f, 1.0f}});
    app->basicUBO.view = renderCamera_calculateViewMatrix(&app->camera);
    app->basicUBO.projection = calculatePerspectiveMatrixFromWindow(app->window);
    app->basicUBO.time = SDL_GetTicks() / 1000.0f;

    clock_tick(&app->clock);
    return appResult;
}

SDL_AppResult application_onRender(Application *app) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (app->currentAS == AS_TITLE_MENU) asTitleMenu_onRender(&app->asTitleMenu, app);
    if (app->currentAS == AS_GAME) asGame_onRender(&app->asGame, app);

    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(app->gpu);

    u32 windowWidth = 0, windowHeight = 0;
    SDL_GPUTexture *windowTexture = NULL;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, app->window, &windowTexture, &windowWidth, &windowHeight)) {
        SDL_Log("Failed to acquire swapchain texture from the window:\n%s", SDL_GetError());
        appResult = SDL_APP_FAILURE;
    }

    if (!windowTexture) {
        goto render_cleanup;
    }

    SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);

    SDL_GPUTransferBufferLocation transferBufferVerticesLocation = {};
    transferBufferVerticesLocation.transfer_buffer = app->transferBuffer;
    transferBufferVerticesLocation.offset = 0;

    SDL_GPUTransferBufferLocation transferBufferIndicesLocation = {};
    transferBufferIndicesLocation.transfer_buffer = app->transferBuffer;
    transferBufferIndicesLocation.offset = app->testMesh.verticesArraySize;

    SDL_GPUBufferRegion vertexBufferRegion = {};
    vertexBufferRegion.buffer = app->vertexBuffer;
    vertexBufferRegion.offset = 0;
    vertexBufferRegion.size = app->testMesh.verticesArraySize;

    SDL_GPUBufferRegion indexBufferRegion = {};
    indexBufferRegion.buffer = app->indexBuffer;
    indexBufferRegion.offset = 0;
    indexBufferRegion.size = app->testMesh.indicesArraySize;

    SDL_UploadToGPUBuffer(copyPass, &transferBufferVerticesLocation, &vertexBufferRegion, false);
    SDL_UploadToGPUBuffer(copyPass, &transferBufferIndicesLocation, &indexBufferRegion, false);

    SDL_EndGPUCopyPass(copyPass);

    SDL_GPUColorTargetInfo colorTargetInfo = {};
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    colorTargetInfo.clear_color = (SDL_FColor){0.125f, 0.125f, 0.25f, 1.0f};
    colorTargetInfo.texture = windowTexture;

    SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);
    SDL_BindGPUGraphicsPipeline(renderPass, app->graphicsPipeline);

    SDL_GPUBufferBinding vertexBufferBindings[1]; SDL_zeroa(vertexBufferBindings);
    vertexBufferBindings[0].buffer = app->vertexBuffer;
    vertexBufferBindings[0].offset = 0;

    SDL_BindGPUVertexBuffers(renderPass, 0, vertexBufferBindings, sizeof(vertexBufferBindings) / sizeof(SDL_GPUBufferBinding));

    SDL_GPUBufferBinding indexBufferBinding = {};
    indexBufferBinding.buffer = app->indexBuffer;
    indexBufferBinding.offset = 0;

    SDL_BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

    SDL_PushGPUVertexUniformData(commandBuffer, 0, &app->basicUBO, sizeof(app->basicUBO));
    SDL_PushGPUFragmentUniformData(commandBuffer, 0, &app->basicUBO, sizeof(app->basicUBO));

    SDL_DrawGPUIndexedPrimitives(renderPass, app->testMesh.indicesArraySize/sizeof(u32), 1, 0, 0, 0);

    SDL_EndGPURenderPass(renderPass);

    render_cleanup:
    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return appResult;
}

SDL_AppResult application_onEvent(Application *app, SDL_Event *event) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (event->type == SDL_EVENT_QUIT) {
        appResult = SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE) {
            appResult = SDL_APP_SUCCESS;
        }

        if (event->key.key == SDLK_Z) {
            sendAppStateEventSwitchState(AS_TITLE_MENU);
        }
        if (event->key.key == SDLK_X) {
            sendAppStateEventSwitchState(AS_GAME);
        }
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        vec2s delta = glms_vec2_zero();
        delta.x = event->motion.xrel;
        delta.y = event->motion.yrel;

        renderCamera_pan(&app->camera, delta, app->clock.dt);
    }

    if (event->user.type == getAppStateEventCode()) {
        if (event->user.code == APP_STATE_EVENT_CODE_SWITCH_STATE) {
            app->nextAS = receiveAppStateEventSwitchState(event);
        }
    }

    if (app->currentAS == AS_TITLE_MENU) asTitleMenu_onEvent(&app->asTitleMenu, event);
    if (app->currentAS == AS_GAME) asGame_onEvent(&app->asGame, event);

    return appResult;
}

SDL_AppResult application_initSDL() {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to init SDL3:\n%s", SDL_GetError());
        appResult = SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        SDL_Log("Failed to init SDL3_ttf:\n%s", SDL_GetError());
        appResult = SDL_APP_FAILURE;
    }

    return appResult;
}

void application_quitSDL() {
    TTF_Quit();
    SDL_Quit();
}

void application_enableRelativeMouseMode(Application *app) {
    SDL_SetWindowRelativeMouseMode(app->window, true);
}

void application_disableRelativeMouseMode(Application *app) {
    SDL_SetWindowRelativeMouseMode(app->window, false);
}

mat4s calculatePerspectiveMatrixFromWindow(SDL_Window *window) {
    s32 width = 0, height = 0; SDL_GetWindowSizeInPixels(window, &width, &height);
    f32 aspectRatio = (f32)width/(f32)height;

    return glms_perspective(45.0f, aspectRatio, 0.1f, 100.0f);
}
