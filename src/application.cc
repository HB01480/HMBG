#include "application.hh"


Application::Application(SDL_AppResult *appResult, int argc, char *argv[])
: window(nullptr)
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

    *appResult = SDL_APP_CONTINUE;
}

Application::~Application() {
    if (!this->window) SDL_DestroyWindow(this->window);

    TTF_Quit();
    SDL_Quit();
}

SDL_AppResult Application::onUpdate() {
    return SDL_APP_CONTINUE;
}

SDL_AppResult Application::onRender() {
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
