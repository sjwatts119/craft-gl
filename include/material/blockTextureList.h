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
    BEDROCK = 4,
    DIAMOND_BLOCK = 5,
    PACKED_ICE = 6,
    COUNT = 7
};

namespace BlockTextureList {
    inline constexpr std::array<const char *, 7> TEXTURES = {
        "../asset/texture/block/goodVibes/grass_block_top.png",
        "../asset/texture/block/goodVibes/grass_block_side.png",
        "../asset/texture/block/goodVibes/dirt.png",
        "../asset/texture/block/goodVibes/stone.png",
        "../asset/texture/block/goodVibes/bedrock.png",
        "../asset/texture/block/goodVibes/diamond_block.png",
        "../asset/texture/block/goodVibes/packed_ice.png",
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
