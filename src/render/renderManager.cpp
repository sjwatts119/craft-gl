#include <render/renderManager.h>

RenderManager::RenderManager() {
    addTestBlocks();
};

void RenderManager::addTestBlocks() {
    Block block;

    // 16x1x16 blocks
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            _blocks.insert({Coordinate {x, 1, z}, block});
        }
    }

    std::cout << "Added Test Blocks - Count: " << _blocks.size() << std::endl;
};

void RenderManager::render() {
    // Foreach of stored types, render.

}

