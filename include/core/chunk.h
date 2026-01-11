#pragma once

#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "geometry/AABB.h"
#include "utility/coordinate.h"
#include "utility/constant.h"
#include "block.h"
#include "utility/blockFace.h"

class ChunkMesh;

class Chunk {
private:
    glm::mat4 _localToWorldMatrix{};

public:
    Coordinate _coordinate;
    AABB _boundingBox{};

    glm::ivec3 _highlightedBlock{-1};

    std::unique_ptr<Block> _blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    std::unique_ptr<ChunkMesh> _mesh;

    Chunk *_leftNeighbour{nullptr};
    Chunk *_rightNeighbour{nullptr};
    Chunk *_downNeighbour{nullptr};
    Chunk *_upNeighbour{nullptr};
    Chunk *_backNeighbour{nullptr};
    Chunk *_frontNeighbour{nullptr};

    explicit Chunk(Coordinate coordinate);

    ~Chunk();

    void generateMesh() const;

    [[nodiscard]] glm::mat4 localToWorldMatrix() const;

    void addTestBlocksBottom();

    void addTestBlocksMiddle();

    void addTestBlocksTop();

    void addTestBlocksAir();

    void addTestBlocksPerlin(const siv::PerlinNoise* perlin);

    void destroyBlock(Coordinate localCoordinate);

    void placeBlock(Coordinate localCoordinate);
};
