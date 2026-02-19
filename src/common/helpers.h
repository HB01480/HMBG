#pragma once
#include "../common.h"


#define FUNCTION_NOT_IMPLEMENTED SDL_Log("Function '%s' is NOT implemented yet", __func__);

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define STMT(x) do { x } while (0)
#define CRASH_PROGRAM() STMT( SDL_assert_always(0) )
