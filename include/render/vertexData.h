#pragma once

#include "geometry/block.h"

struct VertexData {
    float x;
    float y;
    float z;
    BlockType type;

    static size_t size() {
        return sizeof(float) + sizeof(float) + sizeof(float) + sizeof(int);
    }
};
