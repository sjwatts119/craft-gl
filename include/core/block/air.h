#pragma once

#include "block.h"

class Air final : public Block {
public:
    Air(): Block(BlockType::AIR, false, 0.6f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::NONE;
    }
};
