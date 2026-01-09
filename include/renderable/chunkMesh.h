#pragma once

#include <glad/glad.h>

#include "renderable.h"
#include "core/chunk.h"
#include "core/block.h"
#include "utility/coordinate.h"
#include "render/buffer/blockData.h"

class ChunkMesh final : public Renderable {
private:
    Chunk *_chunk;
    bool _dirty = true;

public:
    GLuint _vboId{};
    GLuint _vaoId{};
    GLuint _eboId{};

    glm::ivec3 _highlightedBlockIndex{-1};
    std::vector<BlockData> _vertices;
    std::vector<GLuint> _indices;

    explicit ChunkMesh(Chunk *chunk);

    [[nodiscard]] bool isDirty() const;

    void markAsDirty();

    void markAsDirtyWithNeighbours();

    void regenerateMesh();

    void setHighlightedBlock(glm::ivec3 index);

    void unsetHighlightedBlock();

    void bind() override;

    void render() override;
};
