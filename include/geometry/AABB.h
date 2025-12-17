#pragma once

#include <glm/vec3.hpp>

struct Coordinate;

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

    static AABB forBlock(const Coordinate &coord);

    static AABB forChunk(const Coordinate &chunkCoord);
};
