#include "geometry/block.h"

Block::Block(const BlockType& type) : _type(type) {}

BlockType Block::getType() const {
    return _type;
}

void Block::setType(const BlockType& type) {
    _type = type;
}