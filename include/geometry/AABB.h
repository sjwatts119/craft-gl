#pragma once

#include <core/coordinate.h>
#include <glm/vec3.hpp>
#include <iostream>

struct AABB {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    AABB(const glm::vec3 min, const glm::vec3 max) {
        minX = min.x;
        minY = min.y;
        minZ = min.z;
        maxX = max.x;
        maxY = max.y;
        maxZ = max.z;
    }

    static AABB forBlock(const Coordinate &coord) {
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

    static AABB forChunk(const Coordinate &chunkCoord) {
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
};
