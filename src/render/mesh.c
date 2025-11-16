#include "mesh.h"


RenderMesh renderMesh_init(const RenderVertex *vertices, usize verticesArraySize, const u32 *indices, usize indicesArraySize) {
    RenderMesh mesh; SDL_zero(mesh);
    mesh.verticesArraySize = verticesArraySize;
    mesh.indicesArraySize = indicesArraySize;
    mesh.vertices = memoryDuplicate(vertices, verticesArraySize);
    mesh.indices = memoryDuplicate(indices, indicesArraySize);

    return mesh;
}

void renderMesh_free(RenderMesh *mesh) {
    SDL_free(mesh->vertices);
    SDL_free(mesh->indices);

    mesh->vertices = NULL;
    mesh->indices = NULL;
}
