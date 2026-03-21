#include "core/chunk.h"

#include "core/craft.h"
#include "core/world.h"
#include "render/renderable/chunkMesh.h"

Chunk::Chunk(const Coordinate coordinate) :
    _coordinate(coordinate),
    _worldCoordinate(coordinate.toWorldFromChunk()),
    _boundingBox(AABB::forChunk(coordinate)),
    _mesh{std::make_unique<ChunkMesh>(this)}
{
    _localToWorldMatrix = glm::translate(
        glm::mat4(1.0f),
        _coordinate.toWorldFromChunk().toVec3()
    );
}

Chunk::~Chunk() = default;

void Chunk::generateMesh() const {
    _mesh->regenerateMesh();
}

glm::mat4 Chunk::localToWorldMatrix() const {
    return _localToWorldMatrix;
}

void Chunk::generateBlocks(const siv::PerlinNoise* perlin) {
    for (int x = 0; x < Constant::CHUNK_SIZE; x++) {
        for (int z = 0; z < Constant::CHUNK_SIZE; z++) {
            auto worldCoordinate = _worldCoordinate + Coordinate{x, 0, z};

            const auto targetHeight = Craft::world
                ->terrainHeightAt(worldCoordinate);

            for (int y = 0; y < Constant::CHUNK_SIZE; y++) {
                const Coordinate blockWorldCoordinate = worldCoordinate + Coordinate{0, y, 0};

                if (blockWorldCoordinate.y > targetHeight) {
                    _blocks[x][y][z] = BlockType::AIR;
                } else if (blockWorldCoordinate.y == targetHeight) {
                    _blocks[x][y][z] = BlockType::GRASS;
                } else if (blockWorldCoordinate.y > targetHeight - 5) {
                    _blocks[x][y][z] = BlockType::DIRT;
                } else if (blockWorldCoordinate.y == 0) {
                    _blocks[x][y][z] = BlockType::BEDROCK;
                } else {
                    _blocks[x][y][z] = BlockType::STONE;
                }
            }
        }
    }

    setGenerationStep(GenerationStep::PROTOTYPE);
}

void Chunk::generateDecorations(const siv::PerlinNoise* perlin) {
    std::mt19937 rng(Constant::WORLD_SEED + _coordinate.x + _coordinate.y * 31 + _coordinate.z * 961);
    std::bernoulli_distribution dist(Constant::TREE_GENERATION_CHANCE);

    for (int x = 0; x < Constant::CHUNK_SIZE; x++) {
        for (int z = 0; z < Constant::CHUNK_SIZE; z++) {
            if (!dist(rng)) {
                continue;
            }

            Coordinate localTreeCoordinate {x, 0, z};
            const Coordinate worldTreeCoordinate = _coordinate.toWorldFromChunk(localTreeCoordinate);

            const auto terrainHeight = Craft::world->terrainHeightAt(worldTreeCoordinate);

            if (_worldCoordinate.y < terrainHeight || _worldCoordinate.y > terrainHeight + Constant::CHUNK_SIZE) {
                continue;
            }

            const auto localTerrainHeight = (terrainHeight - _worldCoordinate.y) + 1;

            localTreeCoordinate.y = localTerrainHeight;
            generateTree(localTreeCoordinate);
        }
    }
}

void Chunk::generateTree(const Coordinate localCoordinate) {
    constexpr auto treeHeight = 6;
    constexpr auto leafStartHeight = 4;
    constexpr auto leafHeight = 3;
    constexpr auto tuftHeight = 1;
    constexpr auto peakHeight = 1;

    constexpr auto leafDiameter = 4;
    constexpr auto tuftDiameter = 2;
    constexpr auto peakDiameter = 1;

    for (int y = 0; y < treeHeight; y++) {
        const auto blockCoordinate = localCoordinate + _worldCoordinate + Coordinate{0, y, 0};
        Craft::world->placeBlockQuietly(blockCoordinate, BlockType::OAK_LOG);
    }

    for (int y = leafStartHeight; y < leafStartHeight + leafHeight; y++) {
        for (int x = -leafDiameter / 2; x <= leafDiameter / 2; x++) {
            for (int z = -leafDiameter / 2; z <= leafDiameter / 2; z++) {
                const auto blockCoordinate = localCoordinate + _worldCoordinate + Coordinate{x, y, z};
                Craft::world->placeBlockQuietly(blockCoordinate, BlockType::OAK_LEAVES);
            }
        }
    }

    for (int y = leafStartHeight + leafHeight; y < leafStartHeight + leafHeight + tuftHeight; y++) {
        for (int x = -tuftDiameter / 2; x <= tuftDiameter / 2; x++) {
            for (int z = -tuftDiameter / 2; z <= tuftDiameter / 2; z++) {
                const auto blockCoordinate = localCoordinate + _worldCoordinate + Coordinate{x, y, z};
                Craft::world->placeBlockQuietly(blockCoordinate, BlockType::OAK_LEAVES);
            }
        }
    }

    for (int y = leafStartHeight + leafHeight + tuftHeight; y < leafStartHeight + leafHeight + tuftHeight + peakHeight; y++) {
        for (int x = -peakDiameter / 2; x <= peakDiameter / 2; x++) {
            for (int z = -peakDiameter / 2; z <= peakDiameter / 2; z++) {
                const auto blockCoordinate = localCoordinate + _worldCoordinate + Coordinate{x, y, z};
                Craft::world->placeBlockQuietly(blockCoordinate, BlockType::OAK_LEAVES);
            }
        }
    }
}

/**
 * Destroy a block without marking mesh as dirty.
 */
void Chunk::destroyBlockQuietly(const Coordinate localCoordinate) {
    if (Craft::player->getMovementMode() != MovementMode::FLYING) {
        if (!Block::destructibleFromType(_blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z])) {
            std::cout << "block at " << localCoordinate << " is indestructible." << std::endl;
            return;
        }
    }

    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = BlockType::AIR;
}

void Chunk::destroyBlock(const Coordinate localCoordinate) {
    destroyBlockQuietly(localCoordinate);

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}

/**
 * Place a block without marking mesh as dirty.
 */
void Chunk::placeBlockQuietly(const Coordinate localCoordinate, const BlockType blockType) {
    _blocks[localCoordinate.x][localCoordinate.y][localCoordinate.z] = blockType;
}

void Chunk::placeBlock(const Coordinate localCoordinate, const BlockType blockType) {
    placeBlockQuietly(localCoordinate, blockType);

    _mesh->markAsDirtyWithAffectedNeighbours(localCoordinate);
}

GenerationStep Chunk::getGenerationStep() const {
    return _step;
}

void Chunk::setGenerationStep(const GenerationStep step) {
    _step = step;
}
