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

void ChunkMesh::markAsDirtyWithNeighbours() {
    markAsDirty();

    if (_chunk->_leftNeighbour != nullptr) {
        _chunk->_leftNeighbour->_mesh->markAsDirty();
    }

    if (_chunk->_rightNeighbour != nullptr) {
        _chunk->_rightNeighbour->_mesh->markAsDirty();
    }

    if (_chunk->_upNeighbour != nullptr) {
        _chunk->_upNeighbour->_mesh->markAsDirty();
    }

    if (_chunk->_downNeighbour != nullptr) {
        _chunk->_downNeighbour->_mesh->markAsDirty();
    }

    if (_chunk->_frontNeighbour != nullptr) {
        _chunk->_frontNeighbour->_mesh->markAsDirty();
    }

    if (_chunk->_backNeighbour != nullptr) {
        _chunk->_backNeighbour->_mesh->markAsDirty();
    }
}

void ChunkMesh::regenerateMesh() {
    _vertices.clear();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
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

                bool shouldRenderLeftFace = true;
                bool shouldRenderRightFace = true;
                bool shouldRenderBottomFace = true;
                bool shouldRenderTopFace = true;
                bool shouldRenderBackFace = true;
                bool shouldRenderFrontFace = true;

                if (leftNeighbour.isInBounds()) {
                    shouldRenderLeftFace = _chunk->_blocks[leftNeighbour.x][leftNeighbour.y][leftNeighbour.z].getType() == AIR;
                } else if (_chunk->_leftNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{CHUNK_SIZE - 1, y, z}};
                    shouldRenderLeftFace = _chunk->_leftNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z].getType() == AIR;
                }

                if (rightNeighbour.isInBounds()) {
                    shouldRenderRightFace = _chunk->_blocks[rightNeighbour.x][rightNeighbour.y][rightNeighbour.z].getType() == AIR;
                } else if (_chunk->_rightNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{0, y, z}};
                    shouldRenderRightFace = _chunk->_rightNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z].getType() == AIR;
                }

                if (downNeighbour.isInBounds()) {
                    shouldRenderBottomFace = _chunk->_blocks[downNeighbour.x][downNeighbour.y][downNeighbour.z].getType() == AIR;
                } else if (_chunk->_downNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, CHUNK_SIZE - 1, z}};
                    shouldRenderBottomFace = _chunk->_downNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z].getType() == AIR;
                }

                if (upNeighbour.isInBounds()) {
                    shouldRenderTopFace = _chunk->_blocks[upNeighbour.x][upNeighbour.y][upNeighbour.z].getType() == AIR;
                } else if (_chunk->_upNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, 0, z}};
                    shouldRenderTopFace = _chunk->_upNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z].getType() == AIR;
                }

                if (backNeighbour.isInBounds()) {
                    shouldRenderBackFace = _chunk->_blocks[backNeighbour.x][backNeighbour.y][backNeighbour.z].getType() == AIR;
                } else if (_chunk->_backNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, y, CHUNK_SIZE - 1}};
                    shouldRenderBackFace = _chunk->_backNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z].getType() == AIR;
                }

                if (frontNeighbour.isInBounds()) {
                    shouldRenderFrontFace = _chunk->_blocks[frontNeighbour.x][frontNeighbour.y][frontNeighbour.z].getType() == AIR;
                } else if (_chunk->_frontNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, y, 0}};
                    shouldRenderFrontFace = _chunk->_frontNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z].getType() == AIR;
                }

                if (shouldRenderLeftFace) {
                    for (const auto&[position, normal, texCoords] : Block::getLeftVertices()) {
                        const auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            texCoords.x, texCoords.y,
                            block.getTextureId(FACE_LEFT),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (shouldRenderRightFace) {
                    for (const auto&[position, normal, texCoords] : Block::getRightVertices()) {
                        const auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            texCoords.x, texCoords.y,
                            block.getTextureId(FACE_RIGHT),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (shouldRenderBottomFace) {
                    for (const auto&[position, normal, texCoords] : Block::getBottomVertices()) {
                        const auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            texCoords.x, texCoords.y,
                            block.getTextureId(FACE_BOTTOM),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (shouldRenderTopFace) {
                    for (const auto&[position, normal, texCoords] : Block::getTopVertices()) {
                        const auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            texCoords.x, texCoords.y,
                            block.getTextureId(FACE_TOP),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (shouldRenderBackFace) {
                    for (const auto&[position, normal, texCoords] : Block::getBackVertices()) {
                        const auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            texCoords.x, texCoords.y,
                            block.getTextureId(FACE_BACK),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }

                if (shouldRenderFrontFace) {
                    for (const auto&[position, normal, texCoords] : Block::getFrontVertices()) {
                        const auto localVertices = position + localCoordinate.toVec3();

                        BlockData bufferData {
                            localVertices.x, localVertices.y, localVertices.z,
                            normal.x, normal.y, normal.z,
                            texCoords.x, texCoords.y,
                            block.getTextureId(FACE_FRONT),
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
