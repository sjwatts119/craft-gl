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
        maxX = static_cast<float>(coordinate.x + 1);
        maxY = static_cast<float>(coordinate.y + 1);
        maxZ = static_cast<float>(coordinate.z + 1);
    }
};