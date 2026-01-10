#pragma once
#include <unordered_map>

#include "chunk.h"
#include "utility/coordinate.h"
#include "utility/blockFace.h"
#include "material/light.h"
#include "render/renderable/chunkMesh.h"
#include <siv/perlin.h>


class World {
private:
    Light _sun{
        glm::vec3{0.0f, 255.0f, 0.0f},
        glm::vec3{0.2f, 0.2f, 0.2f},
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.6f, 0.6f, 0.6f)
    };

    siv::PerlinNoise _perlin{WORLD_SEED};
public:
    std::pmr::unordered_map<Coordinate, std::unique_ptr<Chunk>, CoordinateHash> _chunks;

    World();

    void addTestChunks();

    void addPerlinChunks();

    void referenceNeighbours();

    [[nodiscard]] const Block *blockAt(Coordinate worldCoordinate) const;

    void destroyBlock(Coordinate worldCoordinate);

    void placeBlock(Coordinate worldCoordinate);

    void update();

    [[nodiscard]] const Light &getSun() const;
};
