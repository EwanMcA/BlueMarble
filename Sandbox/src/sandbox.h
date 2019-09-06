#pragma once

#include <BlueMarble.h>

#include "GameLayer.h"
#include "RenderLayer.h"

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
        oEntities = std::make_shared<std::vector<BlueMarble::Ref<BlueMarble::Entity>>>();

        PushLayer(new GameLayer(oCamera, oEntities));
        PushLayer(new RenderLayer(oCamera, oEntities));
    }

    virtual ~Sandbox() override
    {

    }

private:
    BlueMarble::Ref<BlueMarble::GameCamera> oCamera;
    std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>> oEntities;
};
