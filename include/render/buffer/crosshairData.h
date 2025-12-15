#pragma once

#include "geometry/block.h"

struct CrosshairData {
    float x, y, z;
    float normalX, normalY, normalZ;

    static size_t size() {
        return
            sizeof(float) + sizeof(float) + sizeof(float) + // pos
            sizeof(float) + sizeof(float) + sizeof(float);  // normal
    }
};
