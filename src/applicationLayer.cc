#include "applicationLayer.hh"


ApplicationLayer::ApplicationLayer(const char *name) {
    if (!name) {
        this->name = SDL_strdup("NULL/DEFAULT-LAYER-NAME");
        SDL_assert_release(this->name);
    }

    this->name = SDL_strdup(name);
    SDL_assert_release(this->name);
}

ApplicationLayer::~ApplicationLayer() {
    delete this->name;
}
