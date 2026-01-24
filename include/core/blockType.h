#pragma once

enum class BlockType: int {
    ERROR = -2,
    AIR = -1,

    GRASS = 0,
    DIRT = 1,
    STONE = 2,
    OAK_LOG = 3,
    OAK_LEAVES = 4,
    OAK_PLANKS = 5,
    DIAMOND_BLOCK = 6,
    PACKED_ICE = 7,
    BEDROCK = 8,
    GLASS_BLOCK = 9,
    COUNT = 10
};