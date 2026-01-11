#include "core/world.h"

#include <ranges>

#include "core/chunk.h"
#include "render/renderable/chunkMesh.h"

World::World() {
    addPerlinChunks();
}

World::~World() = default;

void World::loadChunkColumn(const Coordinate chunkCoordinate) {
    std::vector<Coordinate> coordinates;

    for (int y = 0; y < CHUNK_COUNT_Y; y++) {
        const Coordinate coordinate {chunkCoordinate.x, y, chunkCoordinate.z};
        auto chunk = std::make_unique<Chunk>(this, coordinate);
        _chunks.emplace(coordinate, std::move(chunk));
        coordinates.push_back(coordinate);
    }

    std::vector<std::jthread> threads;

    for (const auto &coordinate : coordinates) {
        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->addTestBlocksPerlin(&_perlin);
            _chunks[coordinate]->_mesh->markAsDirtyWithNeighbours();
        });
    };
}

void World::addPerlinChunks() {
    std::vector<Coordinate> coordinates;

    for (int x = (CHUNK_COUNT_X / -2); x < (CHUNK_COUNT_X / 2); x++) {
        for (int z = CHUNK_COUNT_Z / -2; z < CHUNK_COUNT_Z / 2; z++) {
            for (int y = 0; y < CHUNK_COUNT_Y; y++) {
                const Coordinate coordinate {x, y, z};
                auto chunk = std::make_unique<Chunk>(this, coordinate);
                _chunks.emplace(coordinate, std::move(chunk));
                coordinates.push_back(coordinate);
            }
        }
    }

    std::vector<std::jthread> threads;

    for (const auto &coordinate : coordinates) {
        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->addTestBlocksPerlin(&_perlin);
            _chunks[coordinate]->_mesh->markAsDirty();
        });
    }
}

Block *World::blockAt(const Coordinate worldCoordinate) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        return nullptr;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();
    return chunk->_blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z].get();
}

Chunk *World::chunkAt(const Coordinate chunkCoordinate) const {
    const auto chunkResult = _chunks.find(chunkCoordinate);

    if (chunkResult == _chunks.end()) {
        return nullptr;
    }

    return chunkResult->second.get();
}

void World::destroyBlock(const Coordinate worldCoordinate) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        std::cerr << "No chunk found at " << worldCoordinate << " to destroy block." << std::endl;
        return;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    std::cout << "destroyed " << worldCoordinate << std::endl;
    chunk->destroyBlock(localCoordinate);
}

void World::placeBlock(const Coordinate worldCoordinate) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        std::cerr << "No chunk found at " << worldCoordinate << " to place block." << std::endl;
        return;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    std::cout << "placed " << worldCoordinate << std::endl;
    chunk->placeBlock(localCoordinate);
}

void World::update(const Window *window, const Player *player) {
    // if the player is in a chunk that is not loaded, load the chunk column
    if (const auto playerChunk = player->getChunkCoordinate(); !_chunks.contains(playerChunk)) {
        loadChunkColumn(Coordinate{playerChunk.x, 0, playerChunk.z});
    }

    std::vector<std::jthread> threads;

    // Regenerate chunk meshes for dirty chunks
    for (auto &chunk: _chunks | std::views::values) {
        if (!chunk->_mesh->isDirty()) {
            continue;
        }

        threads.emplace_back([&chunk] {
            chunk->_mesh->regenerateMesh();
        });
    }
}

const Light& World::getSun() const {
    return _sun;
}

