#pragma once
#include "../common.h"
#include "worldConstants.h"
#include "block/block.h"

#include "../render/mesh.h"


typedef struct Chunk {
    ivec3s chunkPosition;

    Block *blocks;

} Chunk;


Chunk chunk_init(ivec3s chunkPosition);
void chunk_free(Chunk *chunk);

void chunk_setBlock(Chunk *chunk, s32 x, s32 y, s32 z, Block block);
Block chunk_getBlock(Chunk *chunk, s32 x, s32 y, s32 z);
RenderMesh chunk_generateSolidMesh(Chunk *chunk);
RenderMesh chunk_generateLiquidMesh(Chunk *chunk);
