#pragma once

#include "block.h"

class Diamond final : public Block {
public:
    Diamond(): Block(BlockType::DIAMOND_BLOCK, true, 0.6f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::DIAMOND_BLOCK;
    }
};
