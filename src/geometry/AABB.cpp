#include "geometry/AABB.h"

AABB AABB::forBlock(const Coordinate &coord) {
    return {
            {
                static_cast<float>(coord.x),
                static_cast<float>(coord.y),
                static_cast<float>(coord.z)
            },
            {
                static_cast<float>(coord.x) + 1.0f,
                static_cast<float>(coord.y) + 1.0f,
                static_cast<float>(coord.z) + 1.0f
            }
    };
}

AABB AABB::forChunk(const Coordinate &chunkCoord) {
    const auto worldCoord = chunkCoord.toWorldFromChunk();

    return {
            {
                static_cast<float>(worldCoord.x),
                static_cast<float>(worldCoord.y),
                static_cast<float>(worldCoord.z)
            },
            {
                static_cast<float>(worldCoord.x) + CHUNK_SIZE,
                static_cast<float>(worldCoord.y) + CHUNK_SIZE,
                static_cast<float>(worldCoord.z) + CHUNK_SIZE
            }
    };
}

AABB AABB::forPlayer(const glm::vec3 &position, const float playerWidth, const float playerHeight) {
    return {
            {
                position.x - (playerWidth / 2),
                position.y,
                position.z - (playerWidth / 2)
            },
            {
                position.x + (playerWidth / 2),
                position.y + playerHeight,
                position.z + (playerWidth / 2)
            }
    };
}

bool AABB::intersects(const AABB &aabb) const {
    return (minX <= aabb.maxX && maxX >= aabb.minX) &&
           (minY <= aabb.maxY && maxY >= aabb.minY) &&
           (minZ <= aabb.maxZ && maxZ >= aabb.minZ);
}