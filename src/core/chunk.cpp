#include "core/chunk.h"

#include "render/renderable/chunkMesh.h"

Chunk::Chunk(World* world, const Coordinate coordinate) :
    _world(world),
    _coordinate(coordinate),
    _boundingBox(AABB::forChunk(coordinate)),
    _mesh{std::make_unique<ChunkMesh>(_world, this)}
{
    _localToWorldMatrix = glm::translate(glm::mat4(1.0f), _coordinate.toWorldFromChunk().toVec3());
}

Chunk::~Chunk() = default;

void Chunk::generateMesh() const {
    _mesh->regenerateMesh();
}

glm::mat4 Chunk::localToWorldMatrix() const {
    return _localToWorldMatrix;
}

void Chunk::addTestBlocksPerlin(const siv::PerlinNoise* perlin) {
    const auto worldCoordinate = _coordinate.toWorldFromChunk();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            const auto currentWorldX = worldCoordinate.x + x;
            const auto currentWorldZ = worldCoordinate.z + z;

            const auto heightSample = perlin->octave2D_01(currentWorldX * 0.001, currentWorldZ * 0.001, 8);
            auto targetHeight = std::floor(heightSample * (WORLD_HEIGHT * CHUNK_SIZE));

            // std::cout << "Target height: " << targetHeight << std::endl;
            // std::cout << "Max chunk height: " << worldCoordinate.y + CHUNK_SIZE << std::endl;

            for (int y = 0; y < CHUNK_SIZE; y++) {
                const Coordinate blockWorldCoordinate {
                    worldCoordinate.x + x,
                    worldCoordinate.y + y,
                    worldCoordinate.z + z,
                };

                if (blockWorldCoordinate.y > targetHeight) {
                    _blocks[x][y][z] = std::make_unique<Block>(BlockType::AIR);
                } else if (blockWorldCoordinate.y == targetHeight) {
                    _blocks[x][y][z] = std::make_unique<Block>(BlockType::GRASS);
                } else if (blockWorldCoordinate.y > targetHeight - 5) {
                    _blocks[x][y][z] = std::make_unique<Block>(BlockType::DIRT);
                } else if (blockWorldCoordinate.y == 0) {
                    _blocks[x][y][z] = std::make_unique<Block>(BlockType::AIR);
                } else {
                    _blocks[x][y][z] = std::make_unique<Block>(BlockType::STONE);
                }
            }
        }
    }
}

void Chunk::destroyBlock(const Coordinate localCoordinate) {
    if (!_blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z]->getDestructible()) {
        std::cout << "block at " << localCoordinate << " is indestructible." << std::endl;
        return;
    }

    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = std::make_unique<Block>(BlockType::AIR);

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}

void Chunk::placeBlock(const Coordinate localCoordinate, const BlockType blockType) {
    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = std::make_unique<Block>(blockType);

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}
