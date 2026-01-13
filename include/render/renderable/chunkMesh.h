#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

#include "renderable.h"
#include "core/block.h"
#include "utility/coordinate.h"
#include "render/buffer/blockData.h"
#include "utility/direction.h"

class Chunk;
class World;

class ChunkMesh final : public Renderable {
private:
    Chunk *_chunk;
    World *_world;
    bool _dirty = true;

public:
    GLuint _vboId{};
    GLuint _vaoId{};
    GLuint _eboId{};

    glm::ivec3 _highlightedBlockIndex{-1};
    std::vector<BlockData> _vertices;
    std::vector<GLuint> _indices;

    explicit ChunkMesh(World* world, Chunk* chunk);

    [[nodiscard]] bool isDirty() const;

    void markAsDirty();

    void markAsDirtyWithNeighbours();

    void markAsDirtyWithAffectedNeighbours(Coordinate localCoordinate);

    void regenerateMesh();

    void setHighlightedBlock(glm::ivec3 index);

    void unsetHighlightedBlock();

    void bind() override;

    void render() override;

    void cleanup() const;
};
