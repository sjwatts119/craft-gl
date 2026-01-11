#pragma once

#include "block.h"

class Bedrock final : public Block {
public:
    Bedrock(): Block(BlockType::BEDROCK, false, 0.6f) {}

    BlockTextureId getTextureId(BlockFace face) override {
        return BlockTextureId::BEDROCK;
    }
};
