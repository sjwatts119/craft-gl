#include "core/world.h"

#include "geometry/chunkMesh.h"

World::World() {
    addTestChunks();
    referenceNeighbours();
}

void World::addTestChunks() {
    for (int x = (CHUNK_COUNT_X / -2); x < (CHUNK_COUNT_X / 2); x++) {
        for (int y = 0; y < CHUNK_COUNT_Y; y++) {
            for (int z = CHUNK_COUNT_Z / -2; z < CHUNK_COUNT_Z / 2; z++) {
                const Coordinate coordinate {x, y, z};

                auto chunk = std::make_unique<Chunk>(coordinate);

                auto globalY = y * CHUNK_SIZE;

                if (globalY == 0) {
                    chunk->addTestBlocksBottom();
                } else if (globalY < CHUNK_SIZE * 4) {
                    chunk->addTestBlocksMiddle();
                } else if (globalY < CHUNK_SIZE * 5) {
                    chunk->addTestBlocksTop();
                } else {
                    chunk->addTestBlocksAir();
                }

                chunk->_mesh->markAsDirty();

                _chunks.emplace(coordinate, std::move(chunk));

                // std::cout << "added test chunk to world at localised position {x: " << x << " y: " << y << " z: " << z << "}" << std::endl;
            }
        }
    }
}

void World::referenceNeighbours() {
    for (int x = (CHUNK_COUNT_X / -2); x < (CHUNK_COUNT_X / 2); x++) {
        for (int y = 0; y < CHUNK_COUNT_Y; y++) {
            for (int z = CHUNK_COUNT_Z / -2; z < CHUNK_COUNT_Z / 2; z++) {
                const Coordinate coordinate {x, y, z};

                for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
                    const BlockFace face {faceIndex};

                    const auto neighbourChunkCoordinate = coordinate.moveTowards(face);

                    auto neighbourChunkResult = _chunks.find(neighbourChunkCoordinate);

                    if (neighbourChunkResult == _chunks.end()) {
                        continue;
                    }

                    Chunk* neighbourChunk = neighbourChunkResult->second.get();

                    switch (face) {
                        case FACE_LEFT:
                            _chunks[coordinate]->_leftNeighbour = neighbourChunk;
                            break;
                        case FACE_RIGHT:
                            _chunks[coordinate]->_rightNeighbour = neighbourChunk;
                            break;
                        case FACE_BOTTOM:
                            _chunks[coordinate]->_downNeighbour = neighbourChunk;
                            break;
                        case FACE_TOP:
                            _chunks[coordinate]->_upNeighbour = neighbourChunk;
                            break;
                        case FACE_BACK:
                            _chunks[coordinate]->_backNeighbour = neighbourChunk;
                            break;
                        case FACE_FRONT:
                            _chunks[coordinate]->_frontNeighbour = neighbourChunk;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

const Block *World::blockAt(const Coordinate worldCoordinate) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();

    const auto chunkResult = _chunks.find(chunkCoordinate);

    if (chunkResult == _chunks.end()) {
        return nullptr;
    }

    const Chunk& chunk = *chunkResult->second;
    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    // std::cout << "World coordinate: {x: " << worldCoordinate.x << " y: " << worldCoordinate.y << " z: " << worldCoordinate.z << "}" << std::endl;
    // std::cout << "Chunk coordinate: {x: " << chunkCoordinate.x << " y: " << chunkCoordinate.y << " z: " << chunkCoordinate.z << "}" << std::endl;
    // std::cout << "Local coordinate: {x: " << localCoordinate.x << " y: " << localCoordinate.y << " z: " << localCoordinate.z << "}" << std::endl;

    return &chunk._blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z];
}

void World::destroyBlock(const Coordinate worldCoordinate) {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();

    const auto chunkResult = _chunks.find(chunkCoordinate);

    if (chunkResult == _chunks.end()) {
        return;
    }

    Chunk& chunk = *chunkResult->second;
    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    std::cout << "destroyed x: " << worldCoordinate.x << " y: " << worldCoordinate.y << " z: " << worldCoordinate.z << std::endl;

    chunk.destroyBlock(localCoordinate);
}

void World::placeBlock(const Coordinate worldCoordinate) {
    const auto chunkResult = _chunks.find(worldCoordinate.toChunkFromWorld());

    if (chunkResult == _chunks.end()) {
        return;
    }

    Chunk& chunk = *chunkResult->second;
    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    std::cout << "placed x: " << worldCoordinate.x << " y: " << worldCoordinate.y << " z: " << worldCoordinate.z << std::endl;

    chunk.placeBlock(localCoordinate);
}

void World::update() {
    // Regenerate chunk meshes for dirty chunks
    for (auto& [chunkCoordinate, chunk] : _chunks) {
        if (!chunk->_mesh->isDirty()) {
            continue;
        }

        chunk->_mesh->regenerateMesh();
    }
}

const Light& World::getSun() const {
    return _sun;
}

