#include "gameLayer.h"

GameLayer::GameLayer(const uint32_t xCount, 
                     const uint32_t yCount,
                     std::shared_ptr<BlueMarble::GameCamera> camera,
                     std::shared_ptr<BlueMarble::EntityComponentSystem> ecs)
    : Layer("GameLayer"), oXCount{ xCount }, oYCount{ yCount }, oCamera(camera), oECS(ecs)
{
    oCamera->Translate(0.0f, -3.2f);
}

void GameLayer::OnUpdate(BlueMarble::TimeStep ts)
{
    if (BlueMarble::Input::IsKeyPressed(BM_KEY_LEFT) ||
        BlueMarble::Input::IsKeyPressed(BM_KEY_A))
    {
        oCamera->Translate(-oCameraMoveSpeed * ts, 0.0f);
    }
    else if (BlueMarble::Input::IsKeyPressed(BM_KEY_RIGHT) ||
        BlueMarble::Input::IsKeyPressed(BM_KEY_D))
    {
        oCamera->Translate(oCameraMoveSpeed * ts, 0.0f);
    }

    if (BlueMarble::Input::IsKeyPressed(BM_KEY_UP) ||
        BlueMarble::Input::IsKeyPressed(BM_KEY_W))
    {
        oCamera->Translate(0.0f, oCameraMoveSpeed * ts);
    }
    else if (BlueMarble::Input::IsKeyPressed(BM_KEY_DOWN) ||
        BlueMarble::Input::IsKeyPressed(BM_KEY_S))
    {
        oCamera->Translate(0.0f, -oCameraMoveSpeed * ts);
    }
}

void GameLayer::OnImGuiRender() 
{
    
}

bool GameLayer::OnMouseScrollEvent(BlueMarble::MouseScrollEvent& event)
{
    oCamera->Zoom(event.GetYOffset());
    return true;
}

void GameLayer::OnEvent(BlueMarble::Event& event)
{
    BlueMarble::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<BlueMarble::MouseScrollEvent>(BM_BIND_EVENT_FN(GameLayer::OnMouseScrollEvent));
}
