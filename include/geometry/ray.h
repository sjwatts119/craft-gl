#pragma once

#include <glm/vec3.hpp>

#include "AABB.h"
#include "blockFace.h"

class Ray {
private:
    glm::vec3 _start;
    glm::vec3 _direction;
    glm::vec3 _inverseDirection;

public:
    static constexpr float RAY_EPSILON = 0.0001f;

    Ray(const glm::vec3 start, const glm::vec3 direction) : _start(start), _direction(direction),
                                                            _inverseDirection(1.0f / direction) {
    }

    // -1.0f for no intersection
    static float distanceToAABB(const AABB &aabb, const Ray &ray) {
        const double tx1 = (aabb.minX - ray._start.x) * ray._inverseDirection.x;
        const double tx2 = (aabb.maxX - ray._start.x) * ray._inverseDirection.x;

        double tMin = std::min(tx1, tx2);
        double tMax = std::max(tx1, tx2);

        const double ty1 = (aabb.minY - ray._start.y) * ray._inverseDirection.y;
        const double ty2 = (aabb.maxY - ray._start.y) * ray._inverseDirection.y;

        tMin = std::max(tMin, std::min(ty1, ty2));
        tMax = std::min(tMax, std::max(ty1, ty2));

        const double tz1 = (aabb.minZ - ray._start.z) * ray._inverseDirection.z;
        const double tz2 = (aabb.maxZ - ray._start.z) * ray._inverseDirection.z;

        tMin = std::max(tMin, std::min(tz1, tz2));
        tMax = std::min(tMax, std::max(tz1, tz2));

        if (tMax >= tMin && tMax >= 0) {
            return tMin >= 0 ? tMin : tMax;
        }

        return -1.0f;
    }

    static BlockFace getHitFace(const AABB &aabb, const Ray &ray, const float distance) {
        const glm::vec3 hitPoint = ray._start + ray._direction * distance;

        if (std::abs(hitPoint.y - aabb.maxY) < RAY_EPSILON) {
            return FACE_TOP;
        }
        if (std::abs(hitPoint.y - aabb.minY) < RAY_EPSILON) {
            return FACE_BOTTOM;
        }
        if (std::abs(hitPoint.z - aabb.minZ) < RAY_EPSILON) {
            return FACE_BACK;
        }
        if (std::abs(hitPoint.z - aabb.maxZ) < RAY_EPSILON) {
            return FACE_FRONT;
        }
        if (std::abs(hitPoint.x - aabb.minX) < RAY_EPSILON) {
            return FACE_LEFT;
        }
        if (std::abs(hitPoint.x - aabb.maxX) < RAY_EPSILON) {
            return FACE_RIGHT;
        }

        return FACE_NONE;
    }
};
