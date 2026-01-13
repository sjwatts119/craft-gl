#include "core/block.h"

Block::Block(const BlockType type)
    : _type(type),
      _highlighted(false),
      _destructible(destructibleFromType(type)),
      _slipperiness(slipperinessFromType(type)) {
}

bool Block::destructibleFromType(const BlockType type) {
    switch (type) {
        case BlockType::BEDROCK:
        case BlockType::AIR:
        case BlockType::ERROR:
            return false;
        default:
            return true;
    }
}

float Block::slipperinessFromType(const BlockType type) {
    switch (type) {
        case BlockType::PACKED_ICE:
            return 0.98f;
        default:
            return 0.6f;
    }
}

BlockTextureId Block::textureIdFromTypeAndFace(const BlockType type, const BlockFace face) {
    switch (type) {
        case BlockType::GRASS:
            switch (face) {
                case FACE_TOP:
                    return BlockTextureId::GRASS_BLOCK_TOP;
                case FACE_BOTTOM:
                    return BlockTextureId::DIRT;
                default:
                    return BlockTextureId::GRASS_BLOCK_SIDE;
            }
        case BlockType::DIRT:
            return BlockTextureId::DIRT;
        case BlockType::STONE:
            return BlockTextureId::STONE;
        case BlockType::BEDROCK:
            return BlockTextureId::BEDROCK;
        case BlockType::DIAMOND_BLOCK:
            return BlockTextureId::DIAMOND_BLOCK;
        case BlockType::PACKED_ICE:
            return BlockTextureId::PACKED_ICE;
        case BlockType::AIR:
        case BlockType::ERROR:
        default:
            return BlockTextureId::NONE;
    }
}

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

BlockTextureId Block::getTextureId(const BlockFace face) const {
    return textureIdFromTypeAndFace(_type, face);
}
