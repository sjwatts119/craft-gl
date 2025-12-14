#pragma once
#include <unordered_map>

#include "chunk.h"
#include "coordinate.h"

class World {
public:
    std::pmr::unordered_map<Coordinate, Chunk, CoordinateHash> _chunks;

    World();

    const Block *blockAt(Coordinate worldCoordinate) const;

    void addTestChunks();
};
