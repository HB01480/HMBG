#pragma once
#include "common.hh"


class ApplicationLayer {
public:
    ApplicationLayer(const char *name) : name(name) {}
    virtual ~ApplicationLayer() = default;

    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    virtual void update(f32 dt) = 0;
    virtual void render(SDL_Renderer *renderer) = 0;
    virtual void processEvent(SDL_Event *event) = 0;

    const char *getName() { return name; }

private:
    const char *name;

};
