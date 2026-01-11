#pragma once

#include "core/block/block.h"

struct CrosshairData {
    float x, y, z;
    float normalX, normalY, normalZ;
    float texU, texV;

    static size_t size() {
        return
                sizeof(float) + sizeof(float) + sizeof(float) + // pos
                sizeof(float) + sizeof(float) + sizeof(float) + // normal
                sizeof(float) + sizeof(float); // tex coords
    }
};
