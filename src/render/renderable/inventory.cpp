#include "render/renderable/inventory.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "core/block.h"
#include "geometry/AABB.h"
#include "geometry/AABB.h"
#include "render/buffer/blockData.h"

class Window;

BlockType Inventory::getSelectedBlockType() const {
    return _selectedBlockType;
}

void Inventory::selectBlockType(const BlockType type) {
    _selectedBlockType = type;
}

void Inventory::selectNextBlockType() {
    auto nextType = static_cast<int>(_selectedBlockType) + 1;

    if (nextType >= static_cast<int>(BlockType::COUNT)) {
        nextType = 0;
    }

    _selectedBlockType = static_cast<BlockType>(nextType);
}

void Inventory::selectPreviousBlockType() {
    auto previousType = static_cast<int>(_selectedBlockType) - 1;

    if (previousType < 0) {
        previousType = static_cast<int>(BlockType::COUNT) - 1;
    }

    _selectedBlockType = static_cast<BlockType>(previousType);
}

