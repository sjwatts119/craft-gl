#include "core/chunk.h"

#include "geometry/blockFace.h"
#include "geometry/chunkMesh.h"

Chunk::Chunk(const Coordinate coordinate)
  : _coordinate(coordinate),
    _blocks{},
    _mesh{std::make_unique<ChunkMesh>(this)}
{
    _localToWorldMatrix = glm::translate(glm::mat4(1.0f), _coordinate.toGlobalFromChunk(Coordinate{0, 0, 0}).toVec3());
    _worldToLocalMatrix = glm::translate(glm::mat4(1.0f), -_coordinate.toGlobalFromChunk(Coordinate{0, 0, 0}).toVec3());

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

glm::mat4 Chunk::worldToLocalMatrix() const {
    return _worldToLocalMatrix;
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

void Chunk::destroyBlock(const Coordinate localCoordinate) {
    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = Block{AIR};

    _mesh->markAsDirtyWithNeighbours();
}

void Chunk::placeBlock(const Coordinate localCoordinate) {
    const Block newBlock{DIAMOND_BLOCK};

    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = newBlock;

    _mesh->markAsDirtyWithNeighbours();
}
