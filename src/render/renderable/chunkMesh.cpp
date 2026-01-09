#include "render/renderable/chunkMesh.h"

ChunkMesh::ChunkMesh(Chunk* chunk) : _chunk(chunk) {
    glGenVertexArrays(1, &_vaoId);
    glGenBuffers(1, &_vboId);
    glGenBuffers(1, &_eboId);

    // reserve for worst case scenario, might be worth being more conservative here later
    _vertices.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 24);
    _indices.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36);
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
    // std::cout << "Regenerating mesh for chunk at coordinate {x: " << _chunk->_coordinate.x << " y: " << _chunk->_coordinate.y << " z: " << _chunk->_coordinate.z << "}" << std::endl;

    _vertices.clear();
    _indices.clear();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                auto block = _chunk->_blocks[x][y][z];

                if (block.getType() == AIR) {
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
                    auto verticesCount = static_cast<GLuint>(_vertices.size());

                    for (const auto index : Block::getLeftIndices()) {
                        _indices.push_back(verticesCount + index);
                    }

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
                    auto verticesCount = static_cast<GLuint>(_vertices.size());
                    for (const auto index : Block::getRightIndices()) {
                        _indices.push_back(verticesCount + index);
                    }

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
                    auto verticesCount = static_cast<GLuint>(_vertices.size());
                    for (const auto index : Block::getBottomIndices()) {
                        _indices.push_back(verticesCount + index);
                    }

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
                    auto currentVertexCount = static_cast<GLuint>(_vertices.size());
                    for (const auto index : Block::getTopIndices()) {
                        _indices.push_back(currentVertexCount + index);
                    }

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
                    auto currentVertexCount = static_cast<GLuint>(_vertices.size());
                    for (const auto index : Block::getBackIndices()) {
                        _indices.push_back(currentVertexCount + index);
                    }

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
                    auto currentVertexCount = static_cast<GLuint>(_vertices.size());
                    for (const auto index : Block::getFrontIndices()) {
                        _indices.push_back(currentVertexCount + index);
                    }

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

void ChunkMesh::bind() {
    glBindVertexArray(_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(BlockData::size() * _vertices.size()), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(_indices.size() * sizeof(GLuint)), _indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 1, GL_INT, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribIPointer(4, 1, GL_INT, static_cast<GLsizei>(BlockData::size()), reinterpret_cast<void *>((8 * sizeof(float)) + sizeof(int)));
    glEnableVertexAttribArray(4);
}

void ChunkMesh::render() {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, static_cast<void *>(nullptr));
}
