#pragma once

#include <unordered_map>
#include <thread>
#include <memory>

#include "utility/coordinate.h"
#include "utility/blockFace.h"
#include "material/light.h"
#include <siv/perlin.h>

#include "player.h"
#include "render/window.h"

class Chunk;
class Block;
class Player;
class ChunkMesh;

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
    std::unordered_map<Coordinate, std::unique_ptr<Chunk>, CoordinateHash> _chunks; // Active chunks
    std::vector<std::unique_ptr<Chunk>> _oldChunks; // Unloaded chunks waiting to be deleted

    World();

    ~World();

    void addInitialChunks();

    void loadChunks(const std::vector<Coordinate> &chunkCoordinates);

    void unloadChunks(const std::vector<Coordinate> &chunkCoordinates);

    [[nodiscard]] Block *blockAt(Coordinate worldCoordinate) const;

    [[nodiscard]] Chunk *chunkAt(Coordinate chunkCoordinate) const;

    void destroyBlock(Coordinate worldCoordinate) const;

    void placeBlock(Coordinate worldCoordinate) const;

    void changeChunks(const Player *player);

    void deleteOldChunks();

    void regenerateDirtyMeshes();

    void update(const Player *player);

    [[nodiscard]] const Light &getSun() const;
};
