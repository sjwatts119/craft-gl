#include <ranges>
#include <render/renderManager.h>

#include "core/chunk.h"
#include "geometry/frustum.h"
#include "render/renderable/debug.h"

RenderManager::RenderManager(const Window* window) {
    // Viewport
    glViewport(0, 0, window->getWidth(), window->getHeight());

    // Clear color (sky blue)
    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    // Depth testing
    glEnable(GL_DEPTH_TEST);

    // Back-face culling
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glLineWidth(3.0f);
};

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::renderBlocks(const Player* player, const Window* window, World* world) const {
    _shaderManager._blockShader.use();

    _textureManager._blockTextures.use(0);
    _shaderManager._blockShader.setInt("uBlockTextures", 0);

    _shaderManager._blockShader.setMat4("uViewMatrix", player->getCamera()->getViewMatrix());
    _shaderManager._blockShader.setMat4("uProjectionMatrix", player->getCamera()->getProjectionMatrix(window));
    _shaderManager._blockShader.setLight("uSun" , world->getSun());

    const Frustum viewFrustum{*player->getCamera(), window->getWidth(), window->getHeight()};

    for (const auto &chunk: world->_chunks | std::views::values) {
        if (!viewFrustum.fastIntersects(chunk->_boundingBox)) {
            continue;
        }

        chunk->_mesh->uploadIfRegenerated();
        chunk->_mesh->bind();

        _shaderManager._blockShader.setMat4("uModelMatrix", chunk->localToWorldMatrix());

        chunk->_mesh->render();
    }
}

void RenderManager::renderDebug(const Player *player, const Window* window, Debug *debug) const {
    debug->upload(); // todo don't upload every frame
    debug->bind();

    _shaderManager._debugShader.use();

    _shaderManager._debugShader.setMat4("uViewMatrix", player->getCamera()->getViewMatrix());
    _shaderManager._debugShader.setMat4("uProjectionMatrix", player->getCamera()->getProjectionMatrix(window));

    debug->render();
}

void RenderManager::renderCrosshair(const Window *window, const Player *player) const {
    player->getCrosshair()->bind();

    _shaderManager._crosshairShader.use();
    _shaderManager._crosshairShader.setMat4("uModelMatrix", Crosshair::localToWorldMatrix(window));

    player->getCrosshair()->render();
}

void RenderManager::renderInventory(const Window* window, const Player* player) const {
    player->getInventory()->bind();

    _shaderManager._inventoryShader.use();

    _textureManager._blockTextures.use(0);
    _shaderManager._inventoryShader.setInt("uBlockTextures", 0);

    _shaderManager._inventoryShader.setLight("uLight", player->getInventory()->getLight());

    _shaderManager._inventoryShader.setMat4("uModelMatrix", player->getInventory()->getModelMatrix());
    _shaderManager._inventoryShader.setMat4("uViewMatrix", Inventory::getViewMatrix());
    _shaderManager._inventoryShader.setMat4("uProjectionMatrix", Inventory::getProjectionMatrix(window));

    player->getInventory()->render();
}

void RenderManager::renderInterface(const Window* window, const Player* player) const {
    glDisable(GL_DEPTH_TEST);

    renderCrosshair(window, player);
    renderInventory(window, player);

    glEnable(GL_DEPTH_TEST);
}

void RenderManager::render(const Player* player, const Window* window, World* world, Debug* debug) const {
    renderBlocks(player, window, world);

    if (player->debugIsEnabled()) {
        renderDebug(player, window, debug);
    }

    renderInterface(window, player);

    glBindVertexArray(0);
}
