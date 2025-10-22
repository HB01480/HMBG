#pragma once
#include "common.hh"


class ApplicationLayer {
public:
    // @param name It should be null-terminated
    ApplicationLayer(const char *name);
    virtual ~ApplicationLayer();

    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    virtual void update(f32 dt) = 0;
    virtual void render(SDL_Renderer *renderer) = 0;
    virtual void processEvent(SDL_Event *event) = 0;

    const char *getName() { return name; }

private:
    char *name;

};
