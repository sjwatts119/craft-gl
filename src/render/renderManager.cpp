#include <ranges>
#include <render/renderManager.h>

#include "core/chunk.h"
#include "core/craft.h"
#include "geometry/frustum.h"
#include "render/renderable/debug.h"

RenderManager::RenderManager() {
    // Viewport
    glViewport(0, 0, Craft::window->getWidth(), Craft::window->getHeight());

    // Clear color (sky blue)
    glClearColor(0.43138f, 0.69412f, 1.0f, 1.0f);

    // Depth testing
    glEnable(GL_DEPTH_TEST);

    // Back-face culling
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glLineWidth(3.0f);
};

void RenderManager::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::renderBlocks() const {
    _shaderManager._blockShader.use();

    _textureManager._blockTextures.use(0);
    _shaderManager._blockShader.setInt("uBlockTextures", 0);

    _shaderManager._blockShader.setMat4("uViewMatrix", Craft::player->getCamera()->getViewMatrix());
    _shaderManager._blockShader.setMat4("uProjectionMatrix", Craft::player->getCamera()->getProjectionMatrix());
    _shaderManager._blockShader.setLight("uSun" , Craft::world->getSun());

    const Frustum viewFrustum{*Craft::player->getCamera()};

    for (const auto &chunk: Craft::world->_chunks | std::views::values) {
        if (!viewFrustum.fastIntersects(chunk->_boundingBox)) {
            continue;
        }

        chunk->_mesh->uploadIfRegenerated();
        chunk->_mesh->bind();

        _shaderManager._blockShader.setMat4("uModelMatrix", chunk->localToWorldMatrix());

        chunk->_mesh->render();
    }
}

void RenderManager::renderDebug() const {
    Craft::debug->upload(); // todo don't upload every frame
    Craft::debug->bind();

    _shaderManager._debugShader.use();

    _shaderManager._debugShader.setMat4("uViewMatrix", Craft::player->getCamera()->getViewMatrix());
    _shaderManager._debugShader.setMat4("uProjectionMatrix", Craft::player->getCamera()->getProjectionMatrix());

    Craft::debug->render();
}

void RenderManager::renderCrosshair() const {
    Craft::player->getCrosshair()->bind();

    _shaderManager._crosshairShader.use();
    _shaderManager._crosshairShader.setMat4("uModelMatrix", Crosshair::localToWorldMatrix());

    Craft::player->getCrosshair()->render();
}

void RenderManager::renderInventory() const {
    Craft::player->getInventory()->uploadIfRegenerated();
    Craft::player->getInventory()->bind();

    _shaderManager._inventoryShader.use();

    _textureManager._blockTextures.use(0);
    _shaderManager._inventoryShader.setInt("uBlockTextures", 0);

    _shaderManager._inventoryShader.setLight("uLight", Craft::player->getInventory()->getLight());

    _shaderManager._inventoryShader.setMat4("uModelMatrix", Craft::player->getInventory()->getModelMatrix());
    _shaderManager._inventoryShader.setMat4("uViewMatrix", Inventory::getViewMatrix());
    _shaderManager._inventoryShader.setMat4("uProjectionMatrix", Inventory::getProjectionMatrix());

    Craft::player->getInventory()->render();
}

void RenderManager::renderInterface() const {
    glDisable(GL_DEPTH_TEST);

    renderCrosshair();
    renderInventory();

    glEnable(GL_DEPTH_TEST);
}

void RenderManager::render() const {
    renderBlocks();

    if (Craft::player->debugIsEnabled()) {
        renderDebug();
    }

    renderInterface();

    glBindVertexArray(0);
}
