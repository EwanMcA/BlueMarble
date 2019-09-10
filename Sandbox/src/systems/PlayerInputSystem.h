#pragma once

#include <BlueMarble.h>

#include "components/components.h"

using BlueMarble::Entity;
using BlueMarble::Ref;

class PlayerInputSystem : public BlueMarble::System 
{
public:
    PlayerInputSystem(std::shared_ptr<BlueMarble::GameCamera> camera)
        : oCamera(camera) {}

    virtual ~PlayerInputSystem() = default;

    virtual void OnUpdate(BlueMarble::TimeStep ts, std::vector<Ref<Entity>>& entities)
    {
        bool leftClick = BlueMarble::Input::IsMouseButtonPressed(0);
        bool rightClick = BlueMarble::Input::IsMouseButtonPressed(1);
        glm::vec3 mousePosition = oCamera->GetWorldCoords(BlueMarble::Input::GetNormMouseX(),
                                                          BlueMarble::Input::GetNormMouseY(),
                                                          BlueMarble::Input::GetNormMouseZ());

        for (BlueMarble::Ref<BlueMarble::Entity> entity : entities)
        {
            if (entity->HasComponent<PlayerInputComponent>())
            {
                auto pInputComp = entity->GetComponent<PlayerInputComponent>();
                pInputComp->leftClick = leftClick;
                pInputComp->rightClick = rightClick;
                pInputComp->mousePosition = mousePosition;
            }
        }
    }

    bool CanPlace(glm::vec3& position);

private:
    std::shared_ptr<BlueMarble::GameCamera> oCamera;
};