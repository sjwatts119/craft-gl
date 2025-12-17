#pragma once

#include <string>
#include <array>
#include <vector>

enum class BlockTextureId : int {
    GRASS_BLOCK_TOP = 0,
    GRASS_BLOCK_SIDE = 1,
    DIRT = 2,
    STONE = 3,
    BEDROCK = 4,
    DIAMOND_BLOCK = 5,
    COUNT = 6
};

namespace BlockTextureList {
    inline constexpr std::array<const char *, 6> TEXTURES = {
        "../asset/texture/block/goodVibes/grass_block_top.png",
        "../asset/texture/block/goodVibes/grass_block_side.png",
        "../asset/texture/block/goodVibes/dirt.png",
        "../asset/texture/block/goodVibes/stone.png",
        "../asset/texture/block/goodVibes/bedrock.png",
        "../asset/texture/block/goodVibes/diamond_block.png"
    };

    inline std::string get(BlockTextureId id) {
        return TEXTURES[static_cast<int>(id)];
    }

    inline std::vector<std::string> allPaths() {
        std::vector<std::string> paths;
        paths.reserve(TEXTURES.size());
        for (const auto &path: TEXTURES) {
            paths.emplace_back(path);
        }
        return paths;
    }
}
