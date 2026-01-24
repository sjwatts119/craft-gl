#pragma once

#include <string>
#include <array>
#include <vector>
#include <optional>

enum class BlockTextureId : int {
    ERROR = -2,
    NONE = -1,
    GRASS_BLOCK_TOP = 0,
    GRASS_BLOCK_SIDE = 1,
    DIRT = 2,
    STONE = 3,
    OAK_LOG_TOP = 4,
    OAK_LOG_SIDE = 5,
    OAK_LEAVES = 6,
    OAK_PLANKS = 7,
    DIAMOND_BLOCK = 8,
    PACKED_ICE = 9,
    BEDROCK = 10,
    GLASS_BLOCK = 11,
    COUNT = 12
};

namespace BlockTextureList {
    inline constexpr std::array<const char *, 12> TEXTURES = {
        "../asset/texture/block/goodVibes/grass_block_top.png",
        "../asset/texture/block/goodVibes/grass_block_side.png",
        "../asset/texture/block/goodVibes/dirt.png",
        "../asset/texture/block/goodVibes/stone.png",
        "../asset/texture/block/goodVibes/oak_log_top.png",
        "../asset/texture/block/goodVibes/oak_log_side.png",
        "../asset/texture/block/goodVibes/oak_leaves.png",
        "../asset/texture/block/goodVibes/oak_planks.png",
        "../asset/texture/block/goodVibes/diamond_block.png",
        "../asset/texture/block/goodVibes/packed_ice.png",
        "../asset/texture/block/goodVibes/bedrock.png",
        "../asset/texture/block/goodVibes/glass_block.png"
    };

    inline std::optional<std::string> get(BlockTextureId id) {
        if (id == BlockTextureId::NONE || id == BlockTextureId::COUNT) {
            return std::nullopt;
        }

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
