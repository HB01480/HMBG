#include "application.hh"


Application::Application(SDL_AppResult *appResult, int argc, char *argv[])
: window(nullptr), gpuDevice(nullptr), clock(),
  gameAS(nullptr), titleMenuAS(nullptr),
  currentAS(nullptr), nextAS(nullptr)
{
    *appResult = initLibraries();

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

    gameAS = new Game_AppState(appResult);
    titleMenuAS = new TitleMenu_AppState(appResult);

    currentAS = titleMenuAS;
    currentAS->enter();

    *appResult = SDL_APP_CONTINUE;
}

Application::~Application() {
    currentAS->exit();

    delete titleMenuAS;
    delete gameAS;

    if (this->gpuDevice) SDL_DestroyGPUDevice(this->gpuDevice);
    if (this->window) SDL_DestroyWindow(this->window);

    quitLibraries();
}

SDL_AppResult Application::onIterate() {
    SDL_AppResult appResult = SDL_APP_CONTINUE;
    appResult = onUpdate();
    appResult = onRender();

    clock.tick();
    return appResult;
}

SDL_AppResult Application::onUpdate() {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    currentAS->onUpdate(clock.dt);
    if (nextAS != nullptr) {
        currentAS->exit();
        nextAS->enter();

        currentAS = nextAS;
        nextAS = nullptr;
    }

    clock.tick();
    return appResult;
}

SDL_AppResult Application::onRender() {
    SDL_AppResult appResult = SDL_APP_CONTINUE;
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

    SDL_GPUColorTargetInfo colorTargetInfo = {
        .clear_color = {32.0f/255.0f, 32.0f/255.0f, 64.0f/255.0f, 255.0f/255.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
        .texture = windowTexture
    };

    SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
    SDL_EndGPURenderPass(renderPass);

    currentAS->onRender(commandBuffer);

    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return appResult;
}

SDL_AppResult Application::onEvent(SDL_Event &event) {
    SDL_AppResult appResult = SDL_APP_CONTINUE;

    if (event.type == SDL_EVENT_QUIT) {
        appResult = SDL_APP_SUCCESS;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            appResult = SDL_APP_SUCCESS;
        }

    }

    currentAS->onEvent(event);

    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_Z) {
            nextAS = titleMenuAS;
        }
        if (event.key.key == SDLK_X) {
            nextAS = gameAS;
        }
    }

    if (event.type == getAppStateEventType()) {
        if (event.user.code == APP_STATE_EVENT_CODE_CHANGE_TO) {
            AppStateID id = receiveASEventChangeTo(event);

            if (id == AppStateID::TitleMenu) nextAS = titleMenuAS;
            else if (id == AppStateID::Game) nextAS = gameAS;
        }
    }

    return appResult;
}

u32 Application::getAppStateEventType() {
    static SDL_AtomicU32 eventType; SDL_zero(eventType);

    SDL_CompareAndSwapAtomicU32(&eventType, 0, SDL_RegisterEvents(1));
    SDL_assert(SDL_GetAtomicU32(&eventType) != 0);

    return SDL_GetAtomicU32(&eventType);
}

void Application::sendASEventChangeTo(AppStateID id) {
    SDL_Event event; SDL_zero(event);
    event.user.type = getAppStateEventType();
    event.user.code = APP_STATE_EVENT_CODE_CHANGE_TO;
    event.user.data1 = new AppStateID(id);
    event.user.data2 = nullptr;

    SDL_PushEvent(&event);
}

AppStateID Application::receiveASEventChangeTo(SDL_Event &event) {
    SDL_assert(event.user.type == getAppStateEventType() && event.user.code == APP_STATE_EVENT_CODE_CHANGE_TO);

    AppStateID id = *(AppStateID *)event.user.data1;

    delete (AppStateID *)event.user.data1;
    return id;
}

SDL_AppResult Application::initLibraries() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void Application::quitLibraries() {
    TTF_Quit();
    SDL_Quit();
}
