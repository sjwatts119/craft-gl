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

class World;
class ChunkMesh;

class Chunk {
private:
    glm::mat4 _localToWorldMatrix{};
    World* _world;

public:
    Coordinate _coordinate;
    AABB _boundingBox;

    glm::ivec3 _highlightedBlock{-1};

    std::unique_ptr<Block> _blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    std::unique_ptr<ChunkMesh> _mesh;

    explicit Chunk(World* world, Coordinate coordinate);

    ~Chunk();

    void generateMesh() const;

    [[nodiscard]] glm::mat4 localToWorldMatrix() const;

    void addTestBlocksPerlin(const siv::PerlinNoise* perlin);

    void destroyBlock(Coordinate localCoordinate);

    void placeBlock(Coordinate localCoordinate, BlockType blockType);
};
