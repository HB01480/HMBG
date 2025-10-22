#include "applicationLayer.hh"


ApplicationLayer::ApplicationLayer(const char *name) {
    if (!name) {
        const char defaultLayerName[] = "NULL/DEFAULT-LAYER-NAME";

        this->name = new char[sizeof(defaultLayerName)];
        SDL_assert_release(this->name);
        SDL_assert_release(SDL_strlcpy(this->name, defaultLayerName, sizeof(defaultLayerName)) == sizeof(defaultLayerName));
    }

    usize bytesToAllocate = SDL_strlen(name) + 1;
    this->name = new char[bytesToAllocate];
    SDL_assert_release(this->name);
}

ApplicationLayer::~ApplicationLayer() {
    delete this->name;
}
