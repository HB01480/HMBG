#include "world.h"


World world_init(u64 seed) {
    World world; SDL_zero(world);
    world.seed = seed;

    return world;
}

void world_free(World *world) {
    // Nothing for now
}

Chunk *world_generateTestChunk(World *world) {
    Chunk *chunk = chunk_init();

    for (u32 x = 0; x < CHUNK_SIZE_X; x++) {
        for (u32 y = 0; y < CHUNK_SIZE_Y; y++) {
            for (u32 z = 0; z < CHUNK_SIZE_Z; z++) {
                chunk_setBlock(chunk, x, y, z, BLOCK_ID_STONE);
            }
        }
    }

    return chunk;
}
