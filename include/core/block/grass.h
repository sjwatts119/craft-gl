#pragma once

#include "block.h"

class Grass final: public Block {
public:
    Grass(): Block(BlockType::GRASS, true, 0.6f) {}

    BlockTextureId getTextureId(const BlockFace face) override {
        switch (face) {
            case FACE_TOP:
                return BlockTextureId::GRASS_BLOCK_TOP;
            case FACE_BOTTOM:
                return BlockTextureId::DIRT;
            default :
                return BlockTextureId::GRASS_BLOCK_SIDE;
        }
    }
};
