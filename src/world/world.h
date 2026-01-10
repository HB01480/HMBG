#pragma once
#include "../common.h"
#include "worldConstants.h"
#include "chunk.h"


typedef struct World {
    u64 seed;

    Chunk chunkList[];

} World;


World world_init(u64 seed);
void world_free(World *world);

void world_generateTestChunk(World *world, ivec3s chunkPosition);
