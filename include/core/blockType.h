#pragma once

enum class BlockType: int {
    ERROR = -2,
    AIR = -1,

    GRASS = 0,
    DIRT = 1,
    STONE = 2,
    BEDROCK = 3,
    DIAMOND_BLOCK = 4,
    PACKED_ICE = 5
};