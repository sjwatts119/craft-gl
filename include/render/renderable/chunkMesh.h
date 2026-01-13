#pragma once

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
    bool _uploadNeeded = false;


public:
    glm::ivec3 _highlightedBlockIndex{-1};
    std::vector<BlockData> _vertices;
    std::vector<GLuint> _indices;

    explicit ChunkMesh(World* world, Chunk* chunk);

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool uploadNeeded() const;

    void markAsDirty();

    void markAsDirtyWithNeighbours();

    void markAsDirtyWithAffectedNeighbours(Coordinate localCoordinate);

    void regenerateMesh();

    void setHighlightedBlock(glm::ivec3 index);

    void unsetHighlightedBlock();

    void uploadIfRegenerated();

    void upload() override;

    void bind() const override;

    void render() const override;

    void cleanup() const override;
};
