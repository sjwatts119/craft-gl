#include "core/world.h"

#include <ranges>
#include <unordered_set>
#include <iostream>

#include "core/chunk.h"
#include "core/craft.h"
#include "render/renderable/chunkMesh.h"

World::World() = default;

World::~World() = default;

void World::init() {
    addInitialChunks();
}

void World::addInitialChunks() {
    const std::vector<Coordinate> visibleCoordinates = Craft::player->getSurroundingChunkCoordinates();
    const std::vector <Coordinate> edgeCoordinates = Craft::player->getSurroundingEdgeChunkCoordinates();

    // Create prototypes
    for (const auto &coordinate : visibleCoordinates) {
        auto chunk = std::make_unique<Chunk>(coordinate);
        _chunks.emplace(coordinate, std::move(chunk));
    }
    for (const auto &coordinate : edgeCoordinates) {
        auto chunk = std::make_unique<Chunk>(coordinate);
        _chunks.emplace(coordinate, std::move(chunk));
    }

    // Generate blocks
    for (const auto &coordinate : visibleCoordinates) {
        _chunks[coordinate]->generateBlocks(&_perlin);
    }
    for (const auto &coordinate : edgeCoordinates) {
        _chunks[coordinate]->generateBlocks(&_perlin);
    }

    // Decorate chunks
    for (const auto &coordinate : visibleCoordinates) {
        _chunks[coordinate]->generateDecorations(&_perlin);
    }
    for (const auto &coordinate : edgeCoordinates) {
        _chunks[coordinate]->generateDecorations(&_perlin);
    }

    // Generate meshes
    for (const auto &coordinate : visibleCoordinates) {
        _chunks[coordinate]->setGenerationStep(GenerationStep::MESHED);
        _chunks[coordinate]->_mesh->markAsDirty();
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

int World::terrainHeightAt(const Coordinate worldCoordinate) const {
    auto heightSample = _perlin.octave2D_01(worldCoordinate.x * 0.001, worldCoordinate.z * 0.001, 8);
    heightSample = std::floor(heightSample * (Constant::MAXIMUM_TERRAIN_HEIGHT - Constant::MINIMUM_TERRAIN_HEIGHT));

    return static_cast<int>(Constant::MINIMUM_TERRAIN_HEIGHT + heightSample);
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

void World::destroyBlockQuietly(const Coordinate worldCoordinate) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        std::cerr << "No chunk found at " << worldCoordinate << " to destroy block." << std::endl;
        return;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    chunk->destroyBlockQuietly(localCoordinate);
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

void World::placeBlockQuietly(const Coordinate worldCoordinate, const BlockType blockType) const {
    const auto chunkCoordinate = worldCoordinate.toChunkFromWorld();
    const auto chunk = chunkAt(chunkCoordinate);

    if (chunk == nullptr) {
        // std::cerr << "No chunk found at " << worldCoordinate << " to place block." << std::endl;
        return;
    }

    const auto localCoordinate = worldCoordinate.toLocalFromWorld();

    chunk->placeBlockQuietly(localCoordinate, blockType);
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
    //
}

void World::update() {
    regenerateDirtyMeshes();
}

const Light& World::getSun() const {
    return _sun;
}