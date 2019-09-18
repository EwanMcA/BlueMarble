#pragma once

#include <BlueMarble.h>

using BlueMarble::Entity;
using BlueMarble::Ref;

class TerritorySystem : public BlueMarble::System
{
public:
    TerritorySystem(std::shared_ptr<BlueMarble::GameCamera> camera)
        : oCamera(camera) {}

    virtual ~TerritorySystem() = default;

    virtual void OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities) override;

    int GetRadius(Ref<Entity> entity) const;
    void Grow(Ref<BlueMarble::Terrain> terrain, int x, int y, int r);

private:
    std::shared_ptr<BlueMarble::GameCamera> oCamera;
};
