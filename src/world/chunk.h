#pragma once
#include "../common.h"
#include "block/block.h"

#include "../render/mesh.h"


#define CHUNK_SIZE 32
#define CHUNK_SIZE_X CHUNK_SIZE
#define CHUNK_SIZE_Y CHUNK_SIZE
#define CHUNK_SIZE_Z CHUNK_SIZE


typedef struct Chunk {
    BlockID blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];

} Chunk;


Chunk *chunk_init();
void chunk_free(Chunk *chunk);

void chunk_set(Chunk *chunk, s32 x, s32 y, s32 z, BlockID block);
BlockID chunk_get(Chunk *chunk, s32 x, s32 y, s32 z);
RenderMesh chunk_generateSolidMesh(Chunk *chunk);
RenderMesh chunk_generateLiquidMesh(Chunk *chunk);
