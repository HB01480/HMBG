#include "chunk.h"


s32 chunk_calculateIndex(s32 x, s32 y, s32 z);


Chunk chunk_init(ivec3s chunkPosition) {
    Chunk chunk; SDL_zero(chunk);
    chunk.chunkPosition = chunkPosition;
    chunk.blocksCount = CHUNK_BLOCKS_COUNT;
    chunk.blocks = SDL_malloc(chunk.blocksCount * sizeof(Block));
    CHECK_ERROR(!chunk.blocks, "Failed to allocate memory for a chunk's block data")

    return chunk;
}

void chunk_free(Chunk *chunk) {
    SDL_free(chunk->blocks);
}

void chunk_setBlock(Chunk *chunk, s32 x, s32 y, s32 z, Block block) {
    s32 index = chunk_calculateIndex(x, y, z);
    chunk->blocks[index] = block;
}

Block chunk_getBlock(Chunk *chunk, s32 x, s32 y, s32 z) {
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
    SDL_assert(x >= 0 && x < CHUNK_SIZE_X);
    SDL_assert(y >= 0 && y < CHUNK_SIZE_Y);
    SDL_assert(z >= 0 && z < CHUNK_SIZE_Z);

    return x * (1) + y * (CHUNK_SIZE_X) + z * (CHUNK_SIZE_X * CHUNK_SIZE_Y);
}
