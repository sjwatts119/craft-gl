#pragma once

#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

#include "coordinate.h"
#include "geometry/block.h"
#include "geometry/chunkMesh.h"

class Chunk {
private:
    Coordinate _coordinate;
    glm::mat4 _localToWorldMatrix{};
    glm::mat4 _worldToLocalMatrix{};
public:
    static constexpr int _size = 16;
    static constexpr int _blockCount = _size * _size * _size;

    Block _blocks[_size][_size][_size];
    // ChunkMesh _mesh;

    explicit Chunk(Coordinate coordinate);

    void generateMesh();

    [[nodiscard]] glm::mat4 localToWorldMatrix() const;

    [[nodiscard]] glm::mat4 worldToLocalMatrix() const;

    void addTestBlocks();
};
