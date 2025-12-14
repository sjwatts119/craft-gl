#pragma once

#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

#include "coordinate.h"
#include "geometry/block.h"

class Chunk {
private:
    Coordinate _coordinate;
    glm::mat4 _localToWorldMatrix{};
public:
    std::pmr::unordered_map<Coordinate, Block, CoordinateHash> _blocks;

    explicit Chunk(Coordinate coordinate);

    glm::mat4 localToWorldMatrix() const;

    void addTestBlocks();
};
