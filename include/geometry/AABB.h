#pragma once

#include <glm/vec3.hpp>
#include <ostream>

struct Coordinate;

struct AABB {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    AABB() {
        minX = 0.0f;
        minY = 0.0f;
        minZ = 0.0f;
        maxX = 0.0f;
        maxY = 0.0f;
        maxZ = 0.0f;
    }

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

inline std::ostream &operator<<(std::ostream &os, const AABB &aabb) {
    os << "{min: {x: " << aabb.minX << ", y: " << aabb.minY << ", z: " << aabb.minZ << "}, "
       << "max: {x: " << aabb.maxX << ", y: " << aabb.maxY << ", z: " << aabb.maxZ << "}}";
    return os;
}
