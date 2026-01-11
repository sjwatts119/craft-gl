#pragma once

#include "block.h"

class PackedIce final: public Block {
public:
    PackedIce(): Block(BlockType::PACKED_ICE, true, 0.98f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::PACKED_ICE;
    }
};
