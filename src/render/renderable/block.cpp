#include "core/block/block.h"

BlockType Block::getType() const {
    return _type;
}

void Block::setType(const BlockType& type) {
    _type = type;
}

float Block::getSlipperinessFactor() const {
    return _slipperiness;
}

void Block::setSlipperinessFactor(const float slipperiness) {
    _slipperiness = slipperiness;
}

bool Block::getDestructible() const {
    return _destructible;
}

void Block::setDestructible(const bool destructible) {
    _destructible = destructible;
}
