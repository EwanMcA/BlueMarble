#pragma once

#include <BlueMarble.h>

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class GameLayer : public BlueMarble::Layer
{
public:
    GameLayer(const uint32_t xCount, 
              const uint32_t yCount,
              std::shared_ptr<BlueMarble::GameCamera> camera,
              std::shared_ptr<BlueMarble::EntityComponentSystem> ecs);

    virtual ~GameLayer() = default;
        
    virtual void OnUpdate(BlueMarble::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(BlueMarble::Event& event) override;

    bool OnMouseScrollEvent(BlueMarble::MouseScrollEvent& event);

private:
    const uint32_t oXCount;
    const uint32_t oYCount;

    std::shared_ptr<BlueMarble::GameCamera> oCamera;
    std::shared_ptr<BlueMarble::EntityComponentSystem> oECS;

    float oCameraMoveSpeed = 5.0f;
};
