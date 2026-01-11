#pragma once

#include "block.h"

class Stone final : public Block {
public:
    Stone(): Block(BlockType::STONE, true, 0.6f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::STONE;
    }
};
