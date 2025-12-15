#pragma once

#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "coordinate.h"
#include "geometry/block.h"

class ChunkMesh;

class Chunk {
private:
    glm::mat4 _localToWorldMatrix{};
    glm::mat4 _worldToLocalMatrix{};
public:
    Coordinate _coordinate;

    static constexpr int _size = 16;
    static constexpr int _blockCount = _size * _size * _size;

    glm::ivec3 highlightedBlock{-1};

    Block _blocks[_size][_size][_size];
    std::unique_ptr<ChunkMesh> _mesh;

    explicit Chunk(Coordinate coordinate);
    ~Chunk();

    void generateMesh() const;

    [[nodiscard]] glm::mat4 localToWorldMatrix() const;

    [[nodiscard]] glm::mat4 worldToLocalMatrix() const;

    void addTestBlocks();

    void destroyBlock(Coordinate localCoordinate);
};
