#pragma once

#include "block.h"

class Dirt final : public Block {
public:
    Dirt(): Block(BlockType::DIRT, true, 0.6f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::DIRT;
    }
};
