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
        minX = static_cast<float>(coordinate.x) - 0.5f;
        minY = static_cast<float>(coordinate.y) - 0.5f;
        minZ = static_cast<float>(coordinate.z) - 0.5f;
        maxX = static_cast<float>(coordinate.x) + 0.5f;
        maxY = static_cast<float>(coordinate.y) + 0.5f;
        maxZ = static_cast<float>(coordinate.z) + 0.5f;
    }
};