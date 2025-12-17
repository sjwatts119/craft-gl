#pragma once

#include "geometry/block.h"

struct BlockData {
    float x, y, z;
    float normalX, normalY, normalZ;
    float texU, texV;
    int textureId;
    int highlighted = 0;

    static size_t size() {
        return
            sizeof(float) + sizeof(float) + sizeof(float) + // pos
            sizeof(float) + sizeof(float) + sizeof(float) + // normal
            sizeof(float) + sizeof(float) + // tex coords
            sizeof(int) + // type
            sizeof(highlighted); // highlighted
    }
};
