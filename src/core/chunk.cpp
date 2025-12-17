#include "core/chunk.h"

#include "geometry/chunkMesh.h"

Chunk::Chunk(const Coordinate coordinate)
  : _coordinate(coordinate),
    _blocks{},
    _mesh{std::make_unique<ChunkMesh>(this)}
{
    _localToWorldMatrix = glm::translate(glm::mat4(1.0f), _coordinate.toGlobalFromChunk(Coordinate{0, 0, 0}).toVec3());
    _worldToLocalMatrix = glm::translate(glm::mat4(1.0f), -_coordinate.toGlobalFromChunk(Coordinate{0, 0, 0}).toVec3());

    addTestBlocks();
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

void Chunk::addTestBlocks() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                BlockType type;

                if (y >= 8) {
                    type = AIR;
                }
                // else if (x == 0 || z == 0 || x == 15 || z == 15) {
                //     type = ERROR;
                // }
                else if (y == 0) {
                    type = BEDROCK;
                }
                else if (y < 4) {
                    type = STONE;
                }
                else if (y < 7) {
                    type = DIRT;
                }
                else {
                    type = GRASS;
                }

                _blocks[x][y][z] = Block{type};
            }
        }
    }

    std::cout << "Added test blocks to chunk at localised position {x: " << _coordinate.x << " z: " << _coordinate.z << "}" << std::endl;
}

void Chunk::destroyBlock(const Coordinate localCoordinate) {
    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = Block{AIR};

    _mesh->markAsDirty();
}
