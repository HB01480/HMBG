#pragma once
#include "../common.h"
#include "worldConstants.h"
#include "chunk.h"


typedef struct World {
    u64 seed;

} World;


World world_init(u64 seed);
void world_free(World *world);

Chunk *world_generateTestChunk(World *world);
