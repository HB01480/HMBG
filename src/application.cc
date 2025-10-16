#include "application.hh"


Application::Application(SDL_AppResult *appResult, int argc, char *argv[])
: window(nullptr), gpuDevice(nullptr)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        *appResult = SDL_APP_FAILURE;
        return;
    }
    if (!TTF_Init()) {
        *appResult = SDL_APP_FAILURE;
        return;
    }

    this->window = SDL_CreateWindow("Highly Moddable Block Game", 1024, 512, 0);
    if (!this->window) {
        SDL_Log("Failed to create window");

        *appResult = SDL_APP_FAILURE;
        return;
    }

    this->gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, HMBG_DEBUG, NULL);
    if (!this->gpuDevice) {
        SDL_Log("Failed to create GPU Device");

        *appResult = SDL_APP_FAILURE;
        return;
    }

    if (!SDL_ClaimWindowForGPUDevice(this->gpuDevice, this->window)) {
        SDL_Log("Failed to claim window for the GPU Device");

        *appResult = SDL_APP_FAILURE;
        return;
    }

    *appResult = SDL_APP_CONTINUE;
}

Application::~Application() {
    if (this->gpuDevice) SDL_DestroyGPUDevice(this->gpuDevice);
    if (this->window) SDL_DestroyWindow(this->window);

    TTF_Quit();
    SDL_Quit();
}

SDL_AppResult Application::onUpdate() {
    return SDL_APP_CONTINUE;
}

SDL_AppResult Application::onRender() {
    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(gpuDevice);

    SDL_GPUTexture *windowTexture;
    u32 windowTextureWidth, windowTextureHeight;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, this->window, &windowTexture, &windowTextureWidth, &windowTextureHeight)) {
        SDL_Log("Failed to acquire the GPU Swapchain Texture for the claimed window");

        SDL_SubmitGPUCommandBuffer(commandBuffer);
        return SDL_APP_FAILURE;
    }

    if (!windowTexture) {
        // It's possible that the window can be minimized. So, that's why it can be NULL.
        // Only if Acquiring it didn't fail first.
        SDL_SubmitGPUCommandBuffer(commandBuffer);
        return SDL_APP_CONTINUE;
    }


    SDL_GPUColorTargetInfo colorTargetInfo = {0};
    colorTargetInfo.clear_color = {32.0f/255.0f, 32.0f/255.0f, 64.0f/255.0f, 255.0f/255.0f};
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    colorTargetInfo.texture = windowTexture;

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);



    SDL_EndGPURenderPass(renderPass);

    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult Application::onEvent(SDL_Event &event) {
    if (event.type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }

    return SDL_APP_CONTINUE;
}
