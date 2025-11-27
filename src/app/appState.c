#include "appState.h"

#include "../common.h"


void logEnterAppState(AppStateID id) {
    SDL_Log("Entered '%s' application state", appStateNames[id]);
}

void logExitAppState(AppStateID id) {
    SDL_Log("Exited '%s' application state", appStateNames[id]);
}

void logCurrentAppState(AppStateID id) {
    SDL_Log("Current application state is '%s'", appStateNames[id]);
}
