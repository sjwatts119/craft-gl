#pragma once

#include <cstddef>
#include "material/blockTextureList.h"

struct BlockData {
    float x, y, z;
    float normalX, normalY, normalZ;
    float texU, texV;
    BlockTextureId texId;
    int highlighted = 0;

    static size_t size() {
        return
                sizeof(float) + sizeof(float) + sizeof(float) + // pos
                sizeof(float) + sizeof(float) + sizeof(float) + // normal
                sizeof(float) + sizeof(float) + // tex coords
                sizeof(int) + // tex ID
                sizeof(highlighted); // highlighted
    }
};
