#pragma once

#include <BlueMarble.h>

#include "gameLayer.h"
#include "mapLayer.h"
#include "systems/renderSystem.h"
#include "systems/citySystem.h"
#include "systems/playerInputSystem.h"

#define X_VERTICES 256
#define Y_VERTICES 256

class Sandbox : public BlueMarble::Application
{
public:
    Sandbox()
        : oCamera(std::make_shared<BlueMarble::GameCamera>(
                     glm::radians(45.0f),
                     (float)BlueMarble::Application::Get().GetWindow().GetWidth() /
                     (float)BlueMarble::Application::Get().GetWindow().GetHeight(),
                     0.1f,
                     100.0f))
    {
        ecs = std::make_shared<BlueMarble::EntityComponentSystem>();
        ecs->AddSystem(new RenderSystem(oCamera));
        ecs->AddSystem(new PlayerInputSystem(oCamera));
        ecs->AddSystem(new CitySystem(oCamera));
        PushLayer(new MapLayer(X_VERTICES, Y_VERTICES, oCamera, ecs));
        PushLayer(new GameLayer(X_VERTICES, Y_VERTICES, oCamera, ecs));
    }

    virtual ~Sandbox() override
    {

    }

private:
    BlueMarble::Ref<BlueMarble::GameCamera> oCamera;
    std::shared_ptr<BlueMarble::EntityComponentSystem> ecs;
};
