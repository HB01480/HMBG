#pragma once


typedef enum AppStateID {
    AS_NULL,

    AS_TITLE_MENU,
    AS_GAME,

} AppStateID;

const static char *appStateNames[] = {
    [AS_NULL] = "NULL",
    [AS_TITLE_MENU] = "Title Menu",
    [AS_GAME] = "Game"

};


void logEnterAppState(AppStateID id);
void logExitAppState(AppStateID id);
void logCurrentAppState(AppStateID id);
