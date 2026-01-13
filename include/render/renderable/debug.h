#pragma once

#include "core/player.h"
#include "core/world.h"
#include "render/buffer/aabbData.h"

class Debug final : public Renderable {
private:
    std::vector<AABBData> _vertices;
    std::vector<GLuint> _indices;

    void addAABB(const AABB& aabb);
public:
    Debug();

    void update(const Player* player, const World* world);

    void upload() override;

    void bind() const override;

    void render() const override;

    void cleanup() const override;
};
