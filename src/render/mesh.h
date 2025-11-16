#pragma once
#include "../common.h"
#include "vertex.h"


typedef struct RenderMesh {
    RenderVertex *vertices;
    usize verticesArraySize;
    u32 *indices;
    usize indicesArraySize;

} RenderMesh;


RenderMesh renderMesh_init(const RenderVertex *vertices, usize verticesArraySize, const u32 *indices, usize indicesArraySize);
void renderMesh_free(RenderMesh *mesh);
