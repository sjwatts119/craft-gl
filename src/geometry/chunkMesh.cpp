#include "geometry/chunkMesh.h"

ChunkMesh::ChunkMesh(Chunk* chunk) : _chunk(chunk) {
    glGenVertexArrays(1, &_vaoId);
    glGenBuffers(1, &_vboId);
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

                auto localCoordinate = Coordinate{glm::vec3{x, y, z}};
                // auto worldCoordinate = Coordinate{_chunk->_coordinate.toVec3() * static_cast<float>(Chunk::_size) + glm::vec3{x, y, z}};
                // glm::mat4 modelToLocalChunkMatrix = glm::translate(glm::mat4{1.0f}, worldCoordinate.toVec3());

                auto leftNeighbour = localCoordinate.leftNeighbour();
                auto rightNeighbour = localCoordinate.rightNeighbour();
                auto downNeighbour = localCoordinate.downNeighbour();
                auto upNeighbour = localCoordinate.upNeighbour();
                auto backNeighbour = localCoordinate.backNeighbour();
                auto frontNeighbour = localCoordinate.frontNeighbour();

                if (!leftNeighbour.isInBounds() || _chunk->_blocks[leftNeighbour.x][leftNeighbour.y][leftNeighbour.z].getType() == AIR) {
                    auto faceVertices = Block::getLeftVertices();

                    for (const auto& vertex : faceVertices) {
                        auto localVertices = vertex + localCoordinate.toVec3();

                        VertexData vertexData {
                            localVertices.x,
                            localVertices.y,
                            localVertices.z,
                            block.getType()
                        };

                        _vertices.push_back(vertexData);
                    }
                }

                if (!rightNeighbour.isInBounds() || _chunk->_blocks[rightNeighbour.x][rightNeighbour.y][rightNeighbour.z].getType() == AIR) {
                    auto faceVertices = Block::getRightVertices();

                    for (const auto& vertex : faceVertices) {
                        auto localVertices = vertex + localCoordinate.toVec3();

                        VertexData vertexData {
                            localVertices.x,
                            localVertices.y,
                            localVertices.z,
                            block.getType()
                        };

                        _vertices.push_back(vertexData);
                    }
                }

                if (!downNeighbour.isInBounds() || _chunk->_blocks[downNeighbour.x][downNeighbour.y][downNeighbour.z].getType() == AIR) {
                    auto faceVertices = Block::getBottomVertices();

                    for (const auto& vertex : faceVertices) {
                        auto localVertices = vertex + localCoordinate.toVec3();

                        VertexData vertexData {
                            localVertices.x,
                            localVertices.y,
                            localVertices.z,
                            block.getType()
                        };

                        _vertices.push_back(vertexData);
                    }
                }

                if (!upNeighbour.isInBounds() || _chunk->_blocks[upNeighbour.x][upNeighbour.y][upNeighbour.z].getType() == AIR) {
                    auto faceVertices = Block::getTopVertices();

                    for (const auto& vertex : faceVertices) {
                        auto localVertices = vertex + localCoordinate.toVec3();

                        VertexData vertexData {
                            localVertices.x,
                            localVertices.y,
                            localVertices.z,
                            block.getType()
                        };

                        _vertices.push_back(vertexData);
                    }
                }

                if (!backNeighbour.isInBounds() || _chunk->_blocks[backNeighbour.x][backNeighbour.y][backNeighbour.z].getType() == AIR) {
                    auto faceVertices = Block::getBackVertices();

                    for (const auto& vertex : faceVertices) {
                        auto localVertices = vertex + localCoordinate.toVec3();

                        VertexData vertexData {
                            localVertices.x,
                            localVertices.y,
                            localVertices.z,
                            block.getType()
                        };

                        _vertices.push_back(vertexData);
                    }
                }

                if (!frontNeighbour.isInBounds() || _chunk->_blocks[frontNeighbour.x][frontNeighbour.y][frontNeighbour.z].getType() == AIR) {
                    auto faceVertices = Block::getFrontVertices();

                    for (const auto& vertex : faceVertices) {
                        auto localVertices = vertex + localCoordinate.toVec3();

                        VertexData vertexData {
                            localVertices.x,
                            localVertices.y,
                            localVertices.z,
                            block.getType()
                        };

                        _vertices.push_back(vertexData);
                    }
                }
            }
        }
    }

    std::cout << "Generated mesh with vertex count: " << _vertices.size() << std::endl;
}
