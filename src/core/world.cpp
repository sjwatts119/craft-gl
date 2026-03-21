#include "core/world.h"

#include <ranges>
#include <unordered_set>
#include <iostream>

#include "core/chunk.h"
#include "core/craft.h"
#include "render/renderable/chunkMesh.h"

World::World() {
    addInitialChunks();
}

World::~World() = default;

void World::addInitialChunks() {
    const std::vector<Coordinate> visibleCoordinates = Craft::player->getSurroundingChunkCoordinates();
    const std::vector <Coordinate> edgeCoordinates = Craft::player->getSurroundingEdgeChunkCoordinates();

    // Create visible chunks
    for (const auto &coordinate : visibleCoordinates) {
        auto chunk = std::make_unique<Chunk>(coordinate);
        _chunks.emplace(coordinate, std::move(chunk));
    }

    // Create edge chunks (invisible)
    for (const auto &coordinate : edgeCoordinates) {
        auto chunk = std::make_unique<Chunk>(coordinate);
        _chunks.emplace(coordinate, std::move(chunk));
    }

    std::vector<std::jthread> threads;

    // Generate blocks for visible chunks
    for (const auto &coordinate : visibleCoordinates) {
        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->generateBlocks(&_perlin);
            _chunks[coordinate]->setGenerationStep(GenerationStep::COMPLETE);
            _chunks[coordinate]->_mesh->markAsDirty();
        });
    }

    // Generate blocks for edge chunks
    for (const auto &coordinate : edgeCoordinates)
    {
        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->generateBlocks(&_perlin);
        });
    }
}

void World::loadChunks(const std::vector<Coordinate>& chunkCoordinates) {
    std::vector<Coordinate> coordinates;

    for (const auto &chunkCoordinate : chunkCoordinates) {
        auto chunk = std::make_unique<Chunk>(chunkCoordinate);
        _chunks.emplace(chunkCoordinate, std::move(chunk));
        coordinates.push_back(chunkCoordinate);
    }

    std::vector<std::jthread> threads;

    for (const auto &coordinate : coordinates) {
        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->generateBlocks(&_perlin);
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

BlockType *World::blockAt(const Coordinate worldCoordinate) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        return nullptr;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();
    return &chunk->_blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z];
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

    chunk->placeBlock(localCoordinate, blockType);
}

void World::changeChunks() {
    const auto threads = std::thread::hardware_concurrency();

    const auto nearbyCoordinates = Craft::player->getSurroundingChunkCoordinates();

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

    const auto deletionsThisFrame = std::min(Constant::CHUNK_DELETIONS_PER_FRAME, static_cast<int>(_oldChunks.size()));

    for (int i = 0; i < deletionsThisFrame; i++) {
        _oldChunks.back()->_mesh->markAsDirtyWithNeighbours();
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
        if (chunk->getGenerationStep() != GenerationStep::COMPLETE && chunk->getGenerationStep() != GenerationStep::MESHED) {
            continue;
        }

        if (!chunk->_mesh->isDirty()) {
            continue;
        }

        if (threads.size() >= threadCount) {
            break;
        }

        threads.emplace_back([this, coordinate] {
            _chunks[coordinate]->_mesh->regenerateMesh();
        });
    }
}

void World::tick() {
    // changeChunks();
}

void World::update() {
    // deleteOldChunks();
    regenerateDirtyMeshes();
}

const Light& World::getSun() const {
    return _sun;
}