#include "render/renderable/chunkMesh.h"

#include "core/chunk.h"
#include "core/block.h"
#include "core/world.h"
#include "utility/direction.h"

ChunkMesh::ChunkMesh(World* world, Chunk* chunk) :
    _chunk(chunk),
    _world(world)
{
    genBuffers();
}

bool ChunkMesh::isDirty() const {
    return _dirty;
}

bool ChunkMesh::uploadNeeded() const {
    return _uploadNeeded;
}

void ChunkMesh::markAsDirty() {
    _dirty = true;
}

void ChunkMesh::markAsDirtyWithNeighbours() {
    markAsDirty();

    auto leftNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_LEFT));
    auto rightNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_RIGHT));
    auto downNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_BOTTOM));
    auto upNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_TOP));
    auto backNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_BACK));
    auto frontNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_FRONT));

    if (leftNeighbour != nullptr) {
        leftNeighbour->_mesh->markAsDirty();
    }
    if (rightNeighbour != nullptr) {
        rightNeighbour->_mesh->markAsDirty();
    }
    if (downNeighbour != nullptr) {
        downNeighbour->_mesh->markAsDirty();
    }
    if (upNeighbour != nullptr) {
        upNeighbour->_mesh->markAsDirty();
    }
    if (backNeighbour != nullptr) {
        backNeighbour->_mesh->markAsDirty();
    }
    if (frontNeighbour != nullptr) {
        frontNeighbour->_mesh->markAsDirty();
    }
}

void ChunkMesh::markAsDirtyWithAffectedNeighbours(const Coordinate localCoordinate) {
    markAsDirty();

    const auto leftNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_LEFT));
    const auto rightNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_RIGHT));
    const auto downNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_BOTTOM));
    const auto upNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_TOP));
    const auto backNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_BACK));
    const auto frontNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_FRONT));

    if (localCoordinate.x == 0 && leftNeighbour != nullptr) {
        leftNeighbour->_mesh->markAsDirty();
    }
    if (localCoordinate.x == CHUNK_SIZE - 1 && rightNeighbour != nullptr) {
        rightNeighbour->_mesh->markAsDirty();
    }
    if (localCoordinate.y == 0 && downNeighbour != nullptr) {
        downNeighbour->_mesh->markAsDirty();
    }
    if (localCoordinate.y == CHUNK_SIZE - 1 && upNeighbour != nullptr) {
        upNeighbour->_mesh->markAsDirty();
    }
    if (localCoordinate.z == 0 && backNeighbour != nullptr) {
        backNeighbour->_mesh->markAsDirty();
    }
    if (localCoordinate.z == CHUNK_SIZE - 1 && frontNeighbour != nullptr) {
        frontNeighbour->_mesh->markAsDirty();
    }
}

void ChunkMesh::regenerateMesh() {
    // std::cout << "Regenerating mesh for chunk at coordinate {x: " << _chunk->_coordinate.x << " y: " << _chunk->_coordinate.y << " z: " << _chunk->_coordinate.z << "}" << std::endl;

    _vertices.clear();
    _indices.clear();

    // get neighbouring chunks
    auto leftNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_LEFT));
    auto rightNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_RIGHT));
    auto downNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_BOTTOM));
    auto upNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_TOP));
    auto backNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_BACK));
    auto frontNeighbour = _world->chunkAt(_chunk->_coordinate.moveTowards(FACE_FRONT));

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                auto blockType = _chunk->_blocks[x][y][z];

                if (blockType == BlockType::AIR) {
                    continue;
                }

                bool isHighlighted = glm::ivec3{x, y, z} == _highlightedBlockIndex;
                auto localCoordinate = Coordinate{glm::vec3{x, y, z}};

                // get neighbouring local coordinates
                auto left = localCoordinate.leftNeighbour();
                auto right = localCoordinate.rightNeighbour();
                auto down = localCoordinate.downNeighbour();
                auto up = localCoordinate.upNeighbour();
                auto back = localCoordinate.backNeighbour();
                auto front = localCoordinate.frontNeighbour();

                bool shouldRenderLeftFace = true;
                bool shouldRenderRightFace = true;
                bool shouldRenderBottomFace = true;
                bool shouldRenderTopFace = true;
                bool shouldRenderBackFace = true;
                bool shouldRenderFrontFace = true;

                if (left.isInBounds()) {
                    shouldRenderLeftFace = _chunk->_blocks[left.x][left.y][left.z] == BlockType::AIR;
                } else if (leftNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{CHUNK_SIZE - 1, y, z}};
                    shouldRenderLeftFace = leftNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z] == BlockType::AIR;
                }

                if (right.isInBounds()) {
                    shouldRenderRightFace = _chunk->_blocks[right.x][right.y][right.z] == BlockType::AIR;
                } else if (rightNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{0, y, z}};
                    shouldRenderRightFace = rightNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z] == BlockType::AIR;
                }

                if (down.isInBounds()) {
                    shouldRenderBottomFace = _chunk->_blocks[down.x][down.y][down.z] == BlockType::AIR;
                } else if (downNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, CHUNK_SIZE - 1, z}};
                    shouldRenderBottomFace = downNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z] == BlockType::AIR;
                }

                if (up.isInBounds()) {
                    shouldRenderTopFace = _chunk->_blocks[up.x][up.y][up.z] == BlockType::AIR;
                } else if (upNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, 0, z}};
                    shouldRenderTopFace = upNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z] == BlockType::AIR;
                }

                if (back.isInBounds()) {
                    shouldRenderBackFace = _chunk->_blocks[back.x][back.y][back.z] == BlockType::AIR;
                } else if (backNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, y, CHUNK_SIZE - 1}};
                    shouldRenderBackFace = backNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z] == BlockType::AIR;
                }

                if (front.isInBounds()) {
                    shouldRenderFrontFace = _chunk->_blocks[front.x][front.y][front.z] == BlockType::AIR;
                } else if (frontNeighbour != nullptr) {
                    const auto localOffset = Coordinate{glm::vec3{x, y, 0}};
                    shouldRenderFrontFace = frontNeighbour->_blocks[localOffset.x][localOffset.y][localOffset.z] == BlockType::AIR;
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
                            // block->getTextureId(FACE_LEFT),
                            Block::textureIdFromTypeAndFace(blockType, FACE_LEFT),
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
                            Block::textureIdFromTypeAndFace(blockType, FACE_RIGHT),
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
                            Block::textureIdFromTypeAndFace(blockType, FACE_BOTTOM),

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
                            Block::textureIdFromTypeAndFace(blockType, FACE_TOP),
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
                            Block::textureIdFromTypeAndFace(blockType, FACE_BACK),
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
                            Block::textureIdFromTypeAndFace(blockType, FACE_FRONT),
                            isHighlighted
                        };

                        _vertices.push_back(bufferData);
                    }
                }
            }
        }
    }

    _dirty = false;
    _uploadNeeded = true;
}

void ChunkMesh::setHighlightedBlock(const glm::ivec3 index) {
    _highlightedBlockIndex = index;

    markAsDirtyWithAffectedNeighbours(Coordinate{index});
}

void ChunkMesh::unsetHighlightedBlock() {
    const auto previousIndex = _highlightedBlockIndex;

    _highlightedBlockIndex = glm::ivec3{-1};

    markAsDirtyWithAffectedNeighbours(Coordinate{previousIndex});
}

void ChunkMesh::uploadIfRegenerated() {
    if (!_uploadNeeded) {
        return;
    }

    upload();
}

void ChunkMesh::upload() {
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

    _uploadNeeded = false;
}

void ChunkMesh::bind() const {
    glBindVertexArray(_vaoId);
}

void ChunkMesh::render() const {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, static_cast<void *>(nullptr));
}

void ChunkMesh::cleanup() const {
    deleteBuffers();
}