#pragma once

#include "block.h"

class Error final : public Block {
public:
    Error(): Block(BlockType::ERROR, true, 0.6f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::NONE;
    }
};
