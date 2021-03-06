#pragma once

#include <BlueMarble.h>

using BlueMarble::Entity;
using BlueMarble::Ref;

class CitySystem : public BlueMarble::System
{
public:
    CitySystem(std::shared_ptr<BlueMarble::GameCamera> camera)
        : oCamera(camera) {}

    virtual ~CitySystem() = default;

    virtual void OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities) override;

    bool CanPlace(BlueMarble::Ref<Entity> terrain, glm::vec3& position);
    
    void PlaceCity(BlueMarble::Ref<Entity> city, glm::vec3& position);

private:
    std::shared_ptr<BlueMarble::GameCamera> oCamera;
};
