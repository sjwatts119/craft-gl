#pragma once
#include <unordered_map>

#include "chunk.h"
#include "coordinate.h"

class World {
public:
    std::pmr::unordered_map<Coordinate, std::unique_ptr<Chunk>, CoordinateHash> _chunks;

    World();

    void addTestChunks();

    const Block *blockAt(Coordinate worldCoordinate) const;

    void destroyBlock(Coordinate worldCoordinate);

    void update();
};
