#include "chunk.h"


s32 chunk_calculateIndex(s32 x, s32 y, s32 z);


Chunk chunk_init(ivec3s chunkPosition) {
    Chunk chunk; SDL_zero(chunk);
    chunk.chunkPosition = chunkPosition;
    chunk.blocks = SDL_malloc(CHUNK_BYTE_SIZE); // This is redundant

    return chunk;
}

void chunk_free(Chunk *chunk) {
    SDL_free(chunk->blocks);
}

void chunk_setBlock(Chunk *chunk, s32 x, s32 y, s32 z, BlockID block) {
    s32 index = chunk_calculateIndex(x, y, z);
    chunk->blocks[index] = block;
}

BlockID chunk_getBlock(Chunk *chunk, s32 x, s32 y, s32 z) {
    s32 index = chunk_calculateIndex(x, y, z);
    return chunk->blocks[index];
}

RenderMesh chunk_generateSolidMesh(Chunk *chunk) {
    FUNCTION_NOT_IMPLEMENTED
    return (RenderMesh){};
}

RenderMesh chunk_generateLiquidMesh(Chunk *chunk) {
    FUNCTION_NOT_IMPLEMENTED
    return (RenderMesh){};
}


s32 chunk_calculateIndex(s32 x, s32 y, s32 z) {
    SDL_assert(x <= CHUNK_SIZE_X && y <= CHUNK_SIZE_Y && z <= CHUNK_SIZE_Z
               && x > 0 && y > 0 && z > 0);

    return x * (1) + y * (CHUNK_SIZE_X) + z * (CHUNK_SIZE_X * CHUNK_SIZE_Y);
}
