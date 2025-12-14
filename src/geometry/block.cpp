#include "geometry/block.h"

BlockType Block::getType() const {
    return _type;
}

void Block::setType(const BlockType& type) {
    _type = type;
}