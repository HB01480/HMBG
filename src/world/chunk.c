#include "chunk.h"


s32 chunk_calculateIndex(s32 x, s32 y, s32 z);


Chunk *chunk_init() {
    Chunk *chunk = SDL_malloc(sizeof(Chunk)); 
    if (!chunk) {
        SDL_Log("Failed to allocate chunk in chunk initialization");
        return NULL;
    }

    SDL_zerop(chunk);
    SDL_zeroa(chunk->blocks); // This is redundant

    return chunk;
}

void chunk_free(Chunk *chunk) {
    SDL_free(chunk);
}

void chunk_set(Chunk *chunk, s32 x, s32 y, s32 z, BlockID block) {
    s32 index = chunk_calculateIndex(x, y, z);
    chunk->blocks[index] = block;
}

BlockID chunk_get(Chunk *chunk, s32 x, s32 y, s32 z) {
    s32 index = chunk_calculateIndex(x, y, z);
    return chunk->blocks[index];
}

RenderMesh chunk_generateSolidMesh(Chunk *chunk) {

}

RenderMesh chunk_generateLiquidMesh(Chunk *chunk) {

}


s32 chunk_calculateIndex(s32 x, s32 y, s32 z) {
    return x * (1) + y * (CHUNK_SIZE_X) + z * (CHUNK_SIZE_X * CHUNK_SIZE_Y);
}
