#pragma once


typedef enum AppStateID {
    AS_NULL,

    AS_TITLE_MENU,
    AS_GAME,

    AS_COUNT_PLUS_ONE
} AppStateID;

#define AS_COUNT AS_COUNT_PLUS_ONE - 1


const static char *appStateNames[] = {
    [AS_NULL] = "NULL",
    [AS_TITLE_MENU] = "Title Menu",
    [AS_GAME] = "Game"

};


void logEnterAppState(AppStateID id);
void logExitAppState(AppStateID id);
void logCurrentAppState(AppStateID id);
