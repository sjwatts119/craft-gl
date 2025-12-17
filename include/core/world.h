#pragma once
#include <unordered_map>

#include "chunk.h"
#include "coordinate.h"
#include "geometry/blockFace.h"
#include "material/light.h"

class World {
private:
    Light _sun {
    glm::vec3 {0.0f, 255.0f, 0.0f},
    glm::vec3 {0.2f, 0.2f, 0.2f},
     glm::vec3(0.3f, 0.3f, 0.3f),
    glm::vec3(0.6f, 0.6f, 0.6f)
    };
public:
    std::pmr::unordered_map<Coordinate, std::unique_ptr<Chunk>, CoordinateHash> _chunks;

    World();

    void addTestChunks();

    void referenceNeighbours();

    const Block *blockAt(Coordinate worldCoordinate) const;

    void destroyBlock(Coordinate worldCoordinate);

    void placeBlock(Coordinate worldCoordinate, BlockFace face);

    void update();

    const Light& getSun() const;
};
