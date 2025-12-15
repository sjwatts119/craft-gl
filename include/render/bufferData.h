#pragma once

#include "vertexData.h"
#include "geometry/block.h"

struct BufferData {
    float x, y, z;
    float normalX, normalY, normalZ;
    BlockType type;
    int highlighted = 0;

    static size_t size() {
        return
            sizeof(float) + sizeof(float) + sizeof(float) + // pos
            sizeof(float) + sizeof(float) + sizeof(float) + // normal
            sizeof(int) + // type
            sizeof(highlighted); // highlighted
    }
};
