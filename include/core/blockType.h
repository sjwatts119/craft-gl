#pragma once

enum class BlockType: int {
    ERROR = -2,
    AIR = -1,

    GRASS = 0,
    DIRT = 1,
    STONE = 2,
    OAK_LOG = 3,
    OAK_PLANKS = 4,
    DIAMOND_BLOCK = 5,
    PACKED_ICE = 6,
    BEDROCK = 7,
    GLASS_BLOCK = 8,
    COUNT = 9
};