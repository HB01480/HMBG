#pragma once
#include "../common.h"
#include "appState.h"


u32 getAppStateEventCode();

#define APP_STATE_EVENT_CODE_SWITCH_STATE 0

void sendAppStateEventSwitchState(AppStateID switchTo);
// PLEASE, use this function to not cause a memory leak with dealing with the event.
// Because if you use the event's send function to send the event, it will allocate
// memory, and the receive function of it will free the memory to prevent a memory leak.
AppStateID receiveAppStateEventSwitchState(SDL_Event *event);
