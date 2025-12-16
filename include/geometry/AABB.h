#pragma once

#include <core/coordinate.h>

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

    explicit AABB(const Coordinate coordinate) {
        minX = static_cast<float>(coordinate.x);
        minY = static_cast<float>(coordinate.y);
        minZ = static_cast<float>(coordinate.z);
        maxX = static_cast<float>(coordinate.x);
        maxY = static_cast<float>(coordinate.y);
        maxZ = static_cast<float>(coordinate.z);
    }

    AABB& adjustToBlockPosition() {
        minX -= 0.5f;
        minY -= 0.5f;
        minZ -= 0.5f;
        maxX += 0.5f;
        maxY += 0.5f;
        maxZ += 0.5f;

        return *this;
    }

    AABB& expandToChunk() {
        maxX += 16;
        maxY += 16;
        maxZ += 16; // TODO USE CHUNK SIZE ON CHUNK CLASS

        return *this;
    }
};
