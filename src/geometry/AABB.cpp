#include "geometry/AABB.h"
#include "core/coordinate.h"
#include "core/staticData.h"

AABB AABB::forBlock(const Coordinate &coord) {
    return {
            {
                static_cast<float>(coord.x) - 0.5f,
                static_cast<float>(coord.y) - 0.5f,
                static_cast<float>(coord.z) - 0.5f
            },
            {
                static_cast<float>(coord.x) + 0.5f,
                static_cast<float>(coord.y) + 0.5f,
                static_cast<float>(coord.z) + 0.5f
            }
    };
}

AABB AABB::forChunk(const Coordinate &chunkCoord) {
    const auto worldCoord = chunkCoord.toWorldFromChunk();

    return {
            {
                static_cast<float>(worldCoord.x) - 0.5f,
                static_cast<float>(worldCoord.y) - 0.5f,
                static_cast<float>(worldCoord.z) - 0.5f
            },
            {
                static_cast<float>(worldCoord.x) + CHUNK_SIZE - 0.5f,
                static_cast<float>(worldCoord.y) + CHUNK_SIZE - 0.5f,
                static_cast<float>(worldCoord.z) + CHUNK_SIZE - 0.5f
            }
    };
}