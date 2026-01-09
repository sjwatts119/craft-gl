#pragma once

#include <glm/vec3.hpp>
#include <ostream>

#include "utility/coordinate.h"

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

    static AABB forPlayer(const glm::vec3 &position, float playerWidth, float playerHeight);

    void expandTo(const glm::vec3 &point);

    void include(AABB &other);

    [[nodiscard]] bool intersects(const AABB &aabb) const;

    [[nodiscard]] bool intersectsX(const AABB &aabb) const;

    [[nodiscard]] bool intersectsY(const AABB &aabb) const;

    [[nodiscard]] bool intersectsZ(const AABB &aabb) const;

    [[nodiscard]] float clipX(const AABB &against, float deltaX) const;

    [[nodiscard]] float clipY(const AABB &against, float deltaY) const;

    [[nodiscard]] float clipZ(const AABB &against, float deltaZ) const;
};

inline std::ostream &operator<<(std::ostream &os, const AABB &aabb) {
    os << "{min: {x: " << aabb.minX << ", y: " << aabb.minY << ", z: " << aabb.minZ << "}, "
       << "max: {x: " << aabb.maxX << ", y: " << aabb.maxY << ", z: " << aabb.maxZ << "}}";
    return os;
}
