#include "core/world.h"

World::World() {
    addTestChunks();
}

const Block *World::blockAt(const Coordinate worldCoordinate) const {
    const auto coord = worldCoordinate.toVec3() / 16.0f;

    const Coordinate chunkCoordinate{coord.x, coord.y, coord.z};

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

void World::addTestChunks() {
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            const Coordinate coordinate {x, 0, z};

            auto chunk = std::make_unique<Chunk>(coordinate);

            _chunks.emplace(coordinate, std::move(chunk));

            std::cout << "added test chunk to world at localised position {x: " << x << " z: " << z << "}" << std::endl;
        }
    }
}
