#pragma once

#include <glad/glad.h>

#include "core/chunk.h"
#include "geometry/block.h"
#include "core/coordinate.h"
#include "render/buffer/blockData.h"

class ChunkMesh {
private:
    Chunk* _chunk;
    bool _dirty = true;
public:
    GLuint _vboId{};
    GLuint _vaoId{};

    glm::ivec3 _highlightedBlockIndex {-1};
    std::vector<BlockData> _vertices;

    explicit ChunkMesh(Chunk* chunk);

    [[nodiscard]] bool isDirty() const;

    void markAsDirty();

    void markAsDirtyWithNeighbours();

    void regenerateMesh();

    void setHighlightedBlock(glm::ivec3 index);

    void unsetHighlightedBlock();

    void bind() const;

    void render() const;
};