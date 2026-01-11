#include "world.h"


World world_init(u64 seed) {
    World world; SDL_zero(world);
    world.seed = seed;
    world.chunkList = NULL;

    return world;
}

void world_free(World *world) {
    SDL_free(world->chunkList);
}

void world_generateTestChunk(World *world, ivec3s chunkPosition) {
    Chunk chunk = chunk_init(chunkPosition);

    for (u32 x = 0; x < CHUNK_SIZE_X; x++) {
        for (u32 y = 0; y < CHUNK_SIZE_Y; y++) {
            for (u32 z = 0; z < CHUNK_SIZE_Z; z++) {
                Block block = block_init(BLOCK_ID_STONE);

                chunk_setBlock(&chunk, x, y, z, block);
            }
        }
    }
}
