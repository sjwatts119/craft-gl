#include "core/chunk.h"

Chunk::Chunk(const Coordinate coordinate) : _coordinate(coordinate) {
    _localToWorldMatrix = glm::translate(glm::mat4(1.0f), _coordinate.toVec3() * 16.0f);

    addTestBlocks();
}

glm::mat4 Chunk::localToWorldMatrix() const {
    return _localToWorldMatrix;
}

void Chunk::addTestBlocks() {
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                BlockType type;

                if (y >= 8) {
                    type = AIR;
                } else if (x == 0 || z == 0 || x == 15 || z == 15) {
                    type = ERROR;
                } else if (y == 0) {
                    type = BEDROCK;
                } else if (y < 4) {
                    type = STONE;
                } else if (y < 7) {
                    type = DIRT;
                } else {
                    type = GRASS;
                }

                _blocks.insert({Coordinate {x, y, z}, Block{type}});
            }
        }
    }

    std::cout << "Added test blocks to chunk at localised position {x: " << _coordinate.x << " z: " << _coordinate.z << "} - Count: " << _blocks.size() << std::endl;
}
