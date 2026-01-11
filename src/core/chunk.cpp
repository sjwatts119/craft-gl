#include "core/chunk.h"

#include "core/block/air.h"
#include "core/block/bedrock.h"
#include "core/block/dirt.h"
#include "core/block/grass.h"
#include "core/block/packedIce.h"
#include "core/block/stone.h"
#include "render/renderable/chunkMesh.h"

Chunk::Chunk(const Coordinate coordinate)
  : _coordinate(coordinate),
    _boundingBox(AABB::forChunk(coordinate)),
    _mesh{std::make_unique<ChunkMesh>(this)}
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
            auto targetHeight = std::floor(heightSample * (CHUNK_COUNT_Y * CHUNK_SIZE));

            // std::cout << "Target height: " << targetHeight << std::endl;
            // std::cout << "Max chunk height: " << worldCoordinate.y + CHUNK_SIZE << std::endl;

            for (int y = 0; y < CHUNK_SIZE; y++) {
                const Coordinate blockWorldCoordinate {
                    worldCoordinate.x + x,
                    worldCoordinate.y + y,
                    worldCoordinate.z + z,
                };

                if (blockWorldCoordinate.y > targetHeight) {
                    _blocks[x][y][z] = std::make_unique<Air>();
                } else if (blockWorldCoordinate.y == targetHeight) {
                    _blocks[x][y][z] = std::make_unique<Grass>();
                } else if (blockWorldCoordinate.y > targetHeight - 5) {
                    _blocks[x][y][z] = std::make_unique<Dirt>();
                } else if (blockWorldCoordinate.y == 0) {
                    _blocks[x][y][z] = std::make_unique<Bedrock>();
                } else {
                    _blocks[x][y][z] = std::make_unique<Stone>();
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

    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = std::make_unique<Air>();

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}

void Chunk::placeBlock(const Coordinate localCoordinate) {
    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = std::make_unique<PackedIce>();

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}
