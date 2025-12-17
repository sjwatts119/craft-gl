#include "core/world.h"

#include "geometry/chunkMesh.h"

World::World() {
    addTestChunks();
}

void World::addTestChunks() {
    for (int x = -4; x < 4; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = -4; z < 4; z++) {
                const Coordinate coordinate {x, y, z};

                auto chunk = std::make_unique<Chunk>(coordinate);

                switch (y) {
                    case 0:
                        chunk->addTestBlocksBottom();
                        break;
                    case 1:
                        chunk->addTestBlocksMiddle();
                        break;
                    case 2:
                        chunk->addTestBlocksTop();
                        break;
                    default:
                        break;
                }

                chunk->_mesh->markAsDirty();

                _chunks.emplace(coordinate, std::move(chunk));

                std::cout << "added test chunk to world at localised position {x: " << x << " y: " << y << " z: " << z << "}" << std::endl;
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
    const Coordinate localCoordinate {xyz(chunk.worldToLocalMatrix() * glm::vec4(worldCoordinate.toVec3(), 1.0f))};

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
    const Coordinate localCoordinate {xyz(chunk.worldToLocalMatrix() * glm::vec4(worldCoordinate.toVec3(), 1.0f))};

    const auto globalCoordinate = chunkCoordinate.toGlobalFromChunk(localCoordinate);
    std::cout << "destroyed x: " << globalCoordinate.x << " y: " << globalCoordinate.y << " z: " << globalCoordinate.z << std::endl;

    chunk.destroyBlock(localCoordinate);
}

void World::placeBlock(const Coordinate worldCoordinate, const BlockFace face) {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();

    const auto chunkResult = _chunks.find(chunkCoordinate);

    if (chunkResult == _chunks.end()) {
        return;
    }

    Chunk& chunk = *chunkResult->second;
    const Coordinate localCoordinate {xyz(chunk.worldToLocalMatrix() * glm::vec4(worldCoordinate.toVec3(), 1.0f))};

    const auto globalCoordinate = chunkCoordinate.toGlobalFromChunk(localCoordinate);
    std::cout << "placed block at x: " << globalCoordinate.x << " y: " << globalCoordinate.y << " z: " << globalCoordinate.z << std::endl;

    chunk.placeBlock(localCoordinate, face);
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

