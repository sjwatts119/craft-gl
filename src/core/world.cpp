#include "core/world.h"

#include <ranges>
#include <unordered_set>
#include <iostream>

#include "core/chunk.h"
#include "render/renderable/chunkMesh.h"

World::World() {
    addInitialChunks();
}

World::~World() = default;

void World::addInitialChunks() {
    std::vector<Coordinate> coordinates;

    for (int x = (RENDER_DISTANCE / -2); x < (RENDER_DISTANCE / 2); x++) {
        for (int z = RENDER_DISTANCE / -2; z < RENDER_DISTANCE / 2; z++) {
            for (int y = 0; y < WORLD_HEIGHT; y++) {
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

void World::loadChunks(const std::vector<Coordinate>& chunkCoordinates) {
    std::vector<Coordinate> coordinates;

    for (const auto &chunkCoordinate : chunkCoordinates) {
        auto chunk = std::make_unique<Chunk>(this, chunkCoordinate);
        _chunks.emplace(chunkCoordinate, std::move(chunk));
        coordinates.push_back(chunkCoordinate);
    }

    std::vector<std::jthread> threads;

    for (const auto &coordinate : coordinates) {
        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->addTestBlocksPerlin(&_perlin);
            _chunks[coordinate]->_mesh->markAsDirtyWithNeighbours();
        });
    };
}

void World::unloadChunks(const std::vector<Coordinate>& chunkCoordinates) {
    for (const auto &chunkCoordinate : chunkCoordinates) {
        if (auto chunk = _chunks.find(chunkCoordinate); chunk != _chunks.end()) {
            _oldChunks.push_back(std::move(chunk->second));
            _chunks.erase(chunk);
        }
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

void World::placeBlock(const Coordinate worldCoordinate, const BlockType blockType) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        std::cerr << "No chunk found at " << worldCoordinate << " to place block." << std::endl;
        return;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    std::cout << "placed " << worldCoordinate << std::endl;
    chunk->placeBlock(localCoordinate, blockType);
}

void World::changeChunks(const Player* player) {
    const auto threads = std::thread::hardware_concurrency();

    const auto nearbyCoordinates = player->getSurroundingChunkCoordinates();

    const std::unordered_set<Coordinate, CoordinateHash> nearbySet(
        nearbyCoordinates.begin(),
        nearbyCoordinates.end()
    );

    std::vector<Coordinate> chunksToLoad;
    std::vector<Coordinate> chunksToUnload;

    for (const auto &coordinate : nearbyCoordinates) {
        if (!_chunks.contains(coordinate)) {
            chunksToLoad.push_back(coordinate);
        }

        if (chunksToLoad.size() >= threads) {
            break;
        }
    }

    for (const auto &coordinate : _chunks | std::views::keys) {
        if (!nearbySet.contains(coordinate)) {
            chunksToUnload.push_back(coordinate);
        }

        if (chunksToUnload.size() >= threads) {
            break;
        }
    }

    if (!chunksToUnload.empty()) {
        unloadChunks(chunksToUnload);
    }

    if (!chunksToLoad.empty()) {
        loadChunks(chunksToLoad);
    }
}

/**
 * Delete some old chunks that have been unloaded.
 */
void World::deleteOldChunks() {
    if (_oldChunks.empty()) {
        return;
    }

    const auto deletionsThisFrame = std::min(CHUNK_DELETIONS_PER_FRAME, static_cast<int>(_oldChunks.size()));

    for (int i = 0; i < deletionsThisFrame; i++) {
        _oldChunks.back()->_mesh->cleanup();
        _oldChunks.pop_back();
    }
}

/**
 * Regenerate some dirty chunk meshes.
 */
void World::regenerateDirtyMeshes() {
    const auto threadCount = std::thread::hardware_concurrency();
    std::vector<std::jthread> threads;

    // Regenerate chunk meshes for dirty chunks
    for (auto &[coordinate, chunk]: _chunks) {
        if (!chunk->_mesh->isDirty()) {
            continue;
        }

        if (threads.size() >= threadCount) {
            continue;
        }

        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->_mesh->regenerateMesh();
        });
    }
}

void World::update(const Player* player) {
    changeChunks(player);
    deleteOldChunks();
    regenerateDirtyMeshes();
}

const Light& World::getSun() const {
    return _sun;
}