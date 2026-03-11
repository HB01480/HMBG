#pragma once
#include "../common.h"
#include "vertex.h"


typedef struct RenderMesh {
    RenderVertex *vertices;
    usize verticesArraySize;
    u32 *indices;
    usize indicesArraySize;

} RenderMesh;


RenderMesh RenderMesh_init(const RenderVertex *vertices, usize verticesArraySize, const u32 *indices, usize indicesArraySize);
void RenderMesh_free(RenderMesh *mesh);
