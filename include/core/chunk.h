#pragma once

#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "geometry/AABB.h"
#include "utility/coordinate.h"
#include "utility/constant.h"
#include "block.h"
#include "utility/blockFace.h"
#include "utility/generationStep.h"

class World;
class ChunkMesh;

class Chunk
{
private:
    glm::mat4 _localToWorldMatrix{};

    GenerationStep _step{GenerationStep::PROTOTYPE};

public:
    Coordinate _coordinate;
    Coordinate _worldCoordinate;
    AABB _boundingBox;

    BlockType _blocks[Constant::CHUNK_SIZE][Constant::CHUNK_SIZE][Constant::CHUNK_SIZE]{BlockType::AIR};
    std::unique_ptr<ChunkMesh> _mesh;

    explicit Chunk(Coordinate coordinate);

    ~Chunk();

    void generateMesh() const;

    [[nodiscard]] glm::mat4 localToWorldMatrix() const;

    void generateBlocks(const siv::PerlinNoise *perlin);

    void generateDecorations(const siv::PerlinNoise *perlin);

    void generateTree(Coordinate localCoordinate);

    void destroyBlockQuietly(Coordinate localCoordinate);

    void destroyBlock(Coordinate localCoordinate);

    void placeBlockQuietly(Coordinate localCoordinate, BlockType blockType);

    void placeBlock(Coordinate localCoordinate, BlockType blockType);

    [[nodiscard]] GenerationStep getGenerationStep() const;

    void setGenerationStep(GenerationStep step);
};