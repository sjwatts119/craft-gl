#include "geometry/chunkMesh.h"

ChunkMesh::ChunkMesh(Chunk* chunk) : _chunk(chunk) {
    glGenVertexArrays(1, &_vaoId);
    glGenBuffers(1, &_vboId);
}

bool ChunkMesh::isDirty() const {
    return _dirty;
}

void ChunkMesh::markAsDirty() {
    _dirty = true;
}

void ChunkMesh::regenerateMesh() {
    _vertices.clear();

    for (int x = 0; x < Chunk::_size; x++) {
        for (int y = 0; y < Chunk::_size; y++) {
            for (int z = 0; z < Chunk::_size; z++) {
                auto block = _chunk->_blocks[x][y][z];
                const auto type = block.getType();

                if (type == AIR) {
                    continue;
                }

                bool isHighlighted = glm::ivec3{x, y, z} == _highlightedBlockIndex;

                auto localCoordinate = Coordinate{glm::vec3{x, y, z}};

                auto leftNeighbour = localCoordinate.leftNeighbour();
                auto rightNeighbour = localCoordinate.rightNeighbour();
                auto downNeighbour = localCoordinate.downNeighbour();
                auto upNeighbour = localCoordinate.upNeighbour();
                auto backNeighbour = localCoordinate.backNeighbour();
                auto frontNeighbour = localCoordinate.frontNeighbour();

                if (!leftNeighbour.isInBounds() || _chunk->_blocks[leftNeighbour.x][leftNeighbour.y][leftNeighbour.z].getType() == AIR) {
                    for (const auto&[position, normal] : Block::getLeftVertices()) {
                        auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            block.getType(),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (!rightNeighbour.isInBounds() || _chunk->_blocks[rightNeighbour.x][rightNeighbour.y][rightNeighbour.z].getType() == AIR) {
                    for (const auto&[position, normal] : Block::getRightVertices()) {
                        auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            block.getType(),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (!downNeighbour.isInBounds() || _chunk->_blocks[downNeighbour.x][downNeighbour.y][downNeighbour.z].getType() == AIR) {
                    for (const auto&[position, normal] : Block::getBottomVertices()) {
                        auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            block.getType(),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (!upNeighbour.isInBounds() || _chunk->_blocks[upNeighbour.x][upNeighbour.y][upNeighbour.z].getType() == AIR) {
                    for (const auto&[position, normal] : Block::getTopVertices()) {
                        auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            block.getType(),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (!backNeighbour.isInBounds() || _chunk->_blocks[backNeighbour.x][backNeighbour.y][backNeighbour.z].getType() == AIR) {
                    for (const auto&[position, normal] : Block::getBackVertices()) {
                        auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            block.getType(),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (!frontNeighbour.isInBounds() || _chunk->_blocks[frontNeighbour.x][frontNeighbour.y][frontNeighbour.z].getType() == AIR) {
                    for (const auto&[position, normal] : Block::getFrontVertices()) {
                        auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            block.getType(),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }
            }
        }
    }

    _dirty = false;

    // std::cout << "Generated mesh with vertex count: " << _vertices.size() << std::endl;
}

void ChunkMesh::setHighlightedBlock(const glm::ivec3 index) {
    _highlightedBlockIndex = index;
}

void ChunkMesh::unsetHighlightedBlock() {
    _highlightedBlockIndex = glm::ivec3{-1};
}
