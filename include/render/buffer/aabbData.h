#pragma once

#include <cstddef>

struct AABBData {
    float x, y, z;

    static size_t size() {
        return
                sizeof(float) + sizeof(float) + sizeof(float);// pos
    }
};
