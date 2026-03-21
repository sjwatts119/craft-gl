#pragma once

enum class BlockType: int {
    ERROR = -1,
    AIR = 0,

    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    OAK_LOG = 4,
    OAK_LEAVES = 5,
    OAK_PLANKS = 6,
    DIAMOND_BLOCK = 7,
    PACKED_ICE = 8,
    BEDROCK = 9,
    GLASS_BLOCK = 10,
    COUNT = 11,
};