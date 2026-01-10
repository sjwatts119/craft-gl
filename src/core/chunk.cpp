#include "core/chunk.h"

#include "render/renderable/chunkMesh.h"

Chunk::Chunk(const Coordinate coordinate)
  : _coordinate(coordinate),
    _boundingBox(AABB::forChunk(coordinate)),
    _blocks{},
    _mesh{std::make_unique<ChunkMesh>(this)}
{
    _localToWorldMatrix = glm::translate(glm::mat4(1.0f), _coordinate.toWorldFromChunk().toVec3());

    // addTestBlocks();
    generateMesh();
}

Chunk::~Chunk() = default;

void Chunk::generateMesh() const {
    _mesh->regenerateMesh();
}

glm::mat4 Chunk::localToWorldMatrix() const {
    return _localToWorldMatrix;
}

void Chunk::addTestBlocksBottom() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType type;

                if (y == 0) {
                    type = BEDROCK;
                } else {
                    type = STONE;
                }

                _blocks[x][y][z] = Block{type};
            }
        }
    }

    // std::cout << "Added test blocks to chunk at localised position {x: " << _coordinate.x << " z: " << _coordinate.z << "}" << std::endl;
}

void Chunk::addTestBlocksMiddle() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType type = STONE;

                _blocks[x][y][z] = Block{type};
            }
        }
    }

    // std::cout << "Added test blocks to chunk at localised position {x: " << _coordinate.x << " z: " << _coordinate.z << "}" << std::endl;
}

void Chunk::addTestBlocksTop() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType type;

                if (y < 3) {
                    type = DIRT;
                } else if (y == 3) {
                    type = GRASS;
                } else {
                    type = AIR;
                }

                _blocks[x][y][z] = Block{type};
            }
        }
    }

    // std::cout << "Added test blocks to chunk at localised position {x: " << _coordinate.x << " z: " << _coordinate.z << "}" << std::endl;
}

void Chunk::addTestBlocksAir() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                _blocks[x][y][z] = Block{AIR};
            }
        }
    }

    // std::cout << "Added test blocks to chunk at localised position {x: " << _coordinate.x << " z: " << _coordinate.z << "}" << std::endl;
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
                    _blocks[x][y][z] = Block{AIR};
                } else if (blockWorldCoordinate.y == targetHeight) {
                    _blocks[x][y][z] = Block{GRASS};
                } else if (blockWorldCoordinate.y > targetHeight - 5) {
                    _blocks[x][y][z] = Block{DIRT};
                } else if (blockWorldCoordinate.y == 0) {
                    _blocks[x][y][z] = Block{BEDROCK};
                } else {
                    _blocks[x][y][z] = Block{STONE};
                }
            }
        }
    }
}


void Chunk::destroyBlock(const Coordinate localCoordinate) {
    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = Block{AIR};

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}

void Chunk::placeBlock(const Coordinate localCoordinate) {
    const Block newBlock{DIAMOND_BLOCK};

    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = newBlock;

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}
