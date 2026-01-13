#pragma once

#include <glm/fwd.hpp>

#include "renderable.h"
#include "core/blockType.h"
#include "render/window.h"

class Inventory final {
private:
    BlockType _selectedBlockType = BlockType::DIAMOND_BLOCK;

public:
    [[nodiscard]] BlockType getSelectedBlockType() const;

    void selectBlockType(BlockType type);

    void selectNextBlockType();

    void selectPreviousBlockType();
};
