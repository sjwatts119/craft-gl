#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "renderable.h"
#include "utility/coordinate.h"
#include "render/buffer/blockData.h"
#include "utility/direction.h"

class Chunk;
class World;

class ChunkMesh final : public Renderable {
private:
    Chunk *_chunk;

    bool _dirty = true;
    bool _uploadNeeded = true;

public:
    std::vector<BlockData> _vertices;
    std::vector<GLuint> _indices;

    explicit ChunkMesh(Chunk *chunk);

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool uploadNeeded() const;

    void markAsDirty();

    void markAsDirtyWithNeighbours();

    void markAsDirtyWithAffectedNeighbours(Coordinate localCoordinate);

    void regenerateMesh();

    void uploadIfRegenerated();

    void upload() override;

    void bind() const override;

    void render() const override;

    void cleanup() const override;
};
