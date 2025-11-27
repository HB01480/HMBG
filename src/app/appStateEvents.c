#include "appStateEvents.h"


u32 getAppStateEventCode() {
    static SDL_AtomicU32 eventType;
    
    if (SDL_CompareAndSwapAtomicU32(&eventType, 0, SDL_RegisterEvents(1))) {
        SDL_Log("The event type for AppState events is %u", SDL_GetAtomicU32(&eventType));
    }

    return SDL_GetAtomicU32(&eventType);
}

void sendAppStateEventSwitchState(AppStateID switchTo) {
    SDL_Event event;
    event.user.type = getAppStateEventCode();
    event.user.code = APP_STATE_EVENT_CODE_SWITCH_STATE;
    event.user.data1 = SDLext_memdup(&switchTo, sizeof(switchTo));
    event.user.data2 = NULL;

    SDL_PushEvent(&event);
}

AppStateID receiveAppStateEventSwitchState(SDL_Event *event) {
    SDL_assert(event->user.type == getAppStateEventCode() && event->user.code == APP_STATE_EVENT_CODE_SWITCH_STATE);

    // NOTE: Naming convention being broken for clarity and readability purposes.
    AppStateID *switchTo_ptr = event->user.data1;
    SDL_assert(switchTo_ptr);
    SDL_free(switchTo_ptr);

    return *switchTo_ptr;
}
