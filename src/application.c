#include "application.h"


static bool isApplicationConstructed = false;

SDL_Surface *application_loadImage(const char *filepath, SDL_Storage *storage);

SDL_AppResult application_init(Application *outApp, int argumentCount, char *arguments[]) {
    HANDLE_CRITICAL(isApplicationConstructed, handleAppFailure, "The application was already successfully constructed")
    HANDLE_CRITICAL(!outApp, handleAppFailure, "Parameter 'outApp' is NULL")

    Application *app = SDL_malloc(sizeof(Application));
    HANDLE_CRITICAL(!app, handleAppFailure, "Failed to allocate memory for the application")
    SDL_zerop(app);

    app->debug = false;
    if (argumentCount > 1) {
        if (SDL_strcmp(arguments[1], "debug") == 0) {
            app->debug = true;
        }
    }

    SDL_WindowFlags windowFlags = SDL_WINDOW_HIDDEN;
    app->window = SDL_CreateWindow("Highly Moddable Block Game", 1024, 512, windowFlags);
    HANDLE_SDL_CRITICAL(!app->window, handleAppFailure, "Failed to construct the window");

    app->gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, app->debug, NULL);
    HANDLE_SDL_CRITICAL(!app->gpu, handleAppFailure, "Failed to construct the gpu device");
    HANDLE_SDL_CRITICAL(!SDL_ClaimWindowForGPUDevice(app->gpu, app->window), handleAppFailure, "Failed to claim the window for the gpu device");

    app->clock = clock_init();

    app->keyState = SDL_GetKeyboardState(NULL);
    app->mouseState = mouseState_init();

    SDL_Storage *contentStorage = SDL_OpenTitleStorage(NULL, 0);
    HANDLE_SDL_CRITICAL(!contentStorage, handleAppFailure, "Failed to open content storage")

    while (!SDL_StorageReady(contentStorage)) {
        SDL_Log("Waiting for content storage to be ready");
        SDL_Delay(1);
    }

    {
        const RenderVertex vertices[] = {
            {.position = { 0.0f,  0.0f,  0.0f}, .texCoord = {1.0f, 1.0f}},
            {.position = { 1.0f,  0.0f,  0.0f}, .texCoord = {0.0f, 1.0f}},
            {.position = { 1.0f,  1.0f,  0.0f}, .texCoord = {0.0f, 0.0f}},
            {.position = { 0.0f,  1.0f,  0.0f}, .texCoord = {1.0f, 0.0f}},
        };
        const u32 indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        app->testMesh = renderMesh_init(vertices, sizeof(vertices), indices, sizeof(indices));
    }

    app->testMesh_modelMatrix = glms_translate_make(glms_vec3_make((f32 []){ 0.0f,  0.0f,  1.0f}));
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
    HANDLE_SDL_CRITICAL(!app->vertexBuffer, handleAppFailure, "Failed to construct the vertex buffer")

    app->indexBuffer = SDL_CreateGPUBuffer(app->gpu, &(SDL_GPUBufferCreateInfo){
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = app->testMesh.indicesArraySize,
        .props = 0
    });
    HANDLE_SDL_CRITICAL(!app->indexBuffer, handleAppFailure, "Failed to construct the index buffer")

    app->transferBuffer = SDL_CreateGPUTransferBuffer(app->gpu, &(SDL_GPUTransferBufferCreateInfo){
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = app->testMesh.verticesArraySize + app->testMesh.indicesArraySize,
        .props = 0
    });
    HANDLE_SDL_CRITICAL(!app->transferBuffer, handleAppFailure, "Failed to construct the transfer buffer")

    app->testImage = application_loadImage("res/images/testImage.png", contentStorage);
    HANDLE_SDL_CRITICAL(!app->testImage, handleAppFailure, "Failed to load the test image from disk")
    app->testImage = SDL_ConvertSurface(app->testImage, SDL_PIXELFORMAT_RGBA32);
    HANDLE_SDL_CRITICAL(!app->testImage, handleAppFailure, "Failed to convert the test image to RGBA32")
    usize testImageSize = app->testImage->w * app->testImage->h * 4;

    app->textureTransferBuffer = SDL_CreateGPUTransferBuffer(app->gpu, &(SDL_GPUTransferBufferCreateInfo){
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = testImageSize,
        .props = 0
    });
    HANDLE_SDL_CRITICAL(!app->textureTransferBuffer, handleAppFailure, "Failed to construct the texture transfer buffer")

    {
        void *transferBufferPtr = SDL_MapGPUTransferBuffer(app->gpu, app->transferBuffer, false);
        HANDLE_SDL_CRITICAL(!transferBufferPtr, handleAppFailure, "Failed to map memory for the transfer buffer")
        void *verticesPtr = transferBufferPtr;
        void *indicesPtr = transferBufferPtr + app->testMesh.verticesArraySize;

        SDL_memcpy(verticesPtr, app->testMesh.vertices, app->testMesh.verticesArraySize);
        SDL_memcpy(indicesPtr, app->testMesh.indices, app->testMesh.indicesArraySize);

        SDL_UnmapGPUTransferBuffer(app->gpu, transferBufferPtr);
    }

    app->testTexture = SDL_CreateGPUTexture(app->gpu, &(SDL_GPUTextureCreateInfo){
        .type = SDL_GPU_TEXTURETYPE_2D,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .width = app->testImage->w,
        .height = app->testImage->h,
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = 0,
        .props = 0,
    });
    HANDLE_SDL_CRITICAL(!app->testTexture, handleAppFailure, "Failed to construct the test texture")

    app->testTextureSampler = SDL_CreateGPUSampler(app->gpu, &(SDL_GPUSamplerCreateInfo){
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .mag_filter = SDL_GPU_FILTER_NEAREST,
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .enable_anisotropy = false,
        .enable_compare = false,
        .compare_op = SDL_GPU_COMPAREOP_INVALID,
        .max_lod = 0.0f,
        .max_anisotropy = 0.0f,
        .min_lod = 0.0f,
        .mip_lod_bias = 0.0f,
        .props = 0
    });
    HANDLE_SDL_CRITICAL(!app->testTextureSampler, handleAppFailure, "Failed to construct the test texture sampler")

    {
        void *textureTransferBufferPtr = SDL_MapGPUTransferBuffer(app->gpu, app->textureTransferBuffer, false);
        HANDLE_SDL_CRITICAL(!textureTransferBufferPtr, handleAppFailure, "Failed to map memory for the texture transfer buffer");
        void *testTexturePtr = textureTransferBufferPtr;

        SDL_memcpy(testTexturePtr, app->testImage->pixels, testImageSize);

        SDL_UnmapGPUTransferBuffer(app->gpu, textureTransferBufferPtr);
    }

    {
        const char *vertexShaderPath = "res/shaders/textured.vert.spv";
        const char *fragmentShaderPath = "res/shaders/textured.frag.spv";
        usize vertexShaderCodeSize = 0, fragmentShaderCodeSize = 0;
        void *vertexShaderCode = NULL;
        void *fragmentShaderCode = NULL;

        vertexShaderCode = SDLext_LoadStorageFile(&vertexShaderCodeSize, contentStorage, vertexShaderPath);
        HANDLE_SDL_CRITICAL(!vertexShaderCode, handleAppFailure, "Failed to load vertex shader code from content storage")

        fragmentShaderCode = SDLext_LoadStorageFile(&fragmentShaderCodeSize, contentStorage, fragmentShaderPath);
        HANDLE_SDL_CRITICAL(!fragmentShaderCode, handleAppFailure, "Failed to load fragment shader code from content storage")

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
        HANDLE_SDL_CRITICAL(!vertexShader, handleAppFailure, "Failed to construct a vertex shader")

        SDL_GPUShader *fragmentShader = SDL_CreateGPUShader(app->gpu, &(SDL_GPUShaderCreateInfo){
            .code = fragmentShaderCode,
            .code_size = fragmentShaderCodeSize,
            .entrypoint = "main",
            .format = SDL_GPU_SHADERFORMAT_SPIRV,
            .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
            .num_samplers = 1,
            .num_storage_buffers = 0,
            .num_storage_textures = 0,
            .num_uniform_buffers = 0,
            .props = 0
        });
        HANDLE_SDL_CRITICAL(!fragmentShader, handleAppFailure, "Failed to construct a fragment shader")

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
                .num_vertex_attributes = 2,
                .vertex_attributes = (SDL_GPUVertexAttribute []){
                    {
                        .buffer_slot = 0,
                        .location = 0,
                        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                        .offset = offsetof(RenderVertex, position),
                    },
                    {
                        .buffer_slot = 0,
                        .location = 1,
                        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                        .offset = offsetof(RenderVertex, texCoord),
                    }
                }
            },

            .depth_stencil_state = {},
            .multisample_state = {},
            .rasterizer_state = {},

            .props = 0
        });
        HANDLE_SDL_CRITICAL(!app->graphicsPipeline, handleAppFailure, "Failed to construct the graphics pipeline")

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
    SDL_ReleaseGPUTransferBuffer(app->gpu, app->textureTransferBuffer);
    SDL_ReleaseGPUGraphicsPipeline(app->gpu, app->graphicsPipeline);

    SDL_free(app->testImage);
    SDL_ReleaseGPUTexture(app->gpu, app->testTexture);
    SDL_ReleaseGPUSampler(app->gpu, app->testTextureSampler);

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
    SDL_UploadToGPUTexture(
        copyPass, &(SDL_GPUTextureTransferInfo){
            .transfer_buffer = app->textureTransferBuffer,
            .offset = 0,
            .pixels_per_row = 0,
            .rows_per_layer = 0
        },
        &(SDL_GPUTextureRegion){
            .texture = app->testTexture,
            .x = 0,
            .y = 0,
            .z = 0,
            .w = app->testImage->w,
            .h = app->testImage->h,
            .d = 1,
            .layer = 0,
            .mip_level = 0
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

    SDL_BindGPUFragmentSamplers(
        renderPass, 0,
        (SDL_GPUTextureSamplerBinding []){
            {
                .texture = app->testTexture,
                .sampler = app->testTextureSampler
            }
        }, 1
    );

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
    CHECK_SDL_ERROR(!SDL_SetWindowRelativeMouseMode(app->window, true), "Failed to enable the relative mode mousing");
}

void application_disableRelativeModeMousing(Application *app) {
    CHECK_SDL_ERROR(!SDL_SetWindowRelativeMouseMode(app->window, false), "Failed to disable the relative mode mousing");
}

mat4s calculatePerspectiveMatrixFromWindow(SDL_Window *window) {
    ivec2s size = glms_ivec2_zero(); SDL_GetWindowSizeInPixels(window, &size.x, &size.y);    
    f32 aspectRatio = (f32)size.x/(f32)size.y;

    return glms_perspective(45.0f * DEGREES_TO_RADIANS, aspectRatio, 0.1f, 100.0f);
}

SDL_Surface *application_loadImage(const char *filepath, SDL_Storage *storage) {
    usize imageBufferSize = 0;
    void *imageBuffer = SDLext_LoadStorageFile(&imageBufferSize, storage, filepath);
    HANDLE_SDL_CRITICAL(!imageBuffer, handleFailure, "Failed to load the image from the provided storage")

    SDL_IOStream *imageIO = SDL_IOFromMem(imageBuffer, imageBufferSize);
    HANDLE_SDL_CRITICAL(!imageIO, handleFailure, "Failed to prepare the io stream around the image buffer")

    SDL_Surface *image = IMG_Load_IO(imageIO, false);
    HANDLE_SDL_CRITICAL(!image, handleFailure, "Failed to load the image from the image IO")
    return image;

    handleFailure:
    return NULL;
}
