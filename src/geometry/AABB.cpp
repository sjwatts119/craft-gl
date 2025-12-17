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
    const auto globalCoord = chunkCoord.toGlobalFromChunk(Coordinate{0, 0, 0});

    return {
            {
                static_cast<float>(globalCoord.x) - 0.5f,
                static_cast<float>(globalCoord.y) - 0.5f,
                static_cast<float>(globalCoord.z) - 0.5f
            },
            {
                static_cast<float>(globalCoord.x) + CHUNK_SIZE - 0.5f,
                static_cast<float>(globalCoord.y) + CHUNK_SIZE - 0.5f,
                static_cast<float>(globalCoord.z) + CHUNK_SIZE - 0.5f
            }
    };
}