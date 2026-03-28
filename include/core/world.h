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

class World
{
private:
    Light _sun{
        glm::vec3{0.0f, 255.0f, 0.0f},
        glm::vec3{0.2f, 0.2f, 0.2f},
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.6f, 0.6f, 0.6f)
    };

    siv::PerlinNoise _perlin{Constant::WORLD_SEED};

public:
    std::unordered_map<Coordinate, std::unique_ptr<Chunk>, CoordinateHash> _chunks; // Active chunks

    World();

    ~World();
    void init();

    void addInitialChunks();
    void loadNewChunks();

    [[nodiscard]] BlockType *blockAt(Coordinate worldCoordinate) const;

    [[nodiscard]] Chunk *chunkAt(Coordinate chunkCoordinate) const;

    [[nodiscard]] int terrainHeightAt(Coordinate worldCoordinate) const;

    void destroyBlockQuietly(Coordinate worldCoordinate) const;

    void destroyBlock(Coordinate worldCoordinate) const;

    void placeBlockQuietly(Coordinate worldCoordinate, BlockType blockType) const;

    void placeBlock(Coordinate worldCoordinate, BlockType blockType) const;

    void regenerateDirtyMeshes();

    void tick();

    void update();

    [[nodiscard]] const Light &getSun() const;
};