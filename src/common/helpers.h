#pragma once
#include "../common.h"


#define FUNCTION_NOT_IMPLEMENTED SDL_Log("Function '%s' is NOT implemented yet", __func__);

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
