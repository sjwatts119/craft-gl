#pragma once

#include <glad/glad.h>

#include "core/chunk.h"
#include "geometry/block.h"
#include "core/coordinate.h"
#include "render/vertexData.h"

class ChunkMesh {
private:
    Chunk* _chunk;
public:
    GLuint _vboId{};
    GLuint _vaoId{};

    std::vector<VertexData> _vertices;

    explicit ChunkMesh(Chunk* chunk);

    void regenerateMesh();
};