#pragma once

#include <BlueMarble.h>

using BlueMarble::Entity;
using BlueMarble::Ref;

class RenderSystem : public BlueMarble::System
{
public:
    RenderSystem(std::shared_ptr<BlueMarble::GameCamera> camera)
        : oCamera(camera) {}

    virtual ~RenderSystem() = default;

    virtual void OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities) override;

private:
    std::shared_ptr<BlueMarble::GameCamera> oCamera;
};
