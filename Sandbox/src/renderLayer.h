#pragma once

#include <BlueMarble.h>

class RenderLayer : public BlueMarble::Layer
{
public:
    RenderLayer(std::shared_ptr<BlueMarble::GameCamera> camera, 
                std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>> entities)
        : Layer("RenderLayer"), oCamera(camera), oEntities(entities) {}

    virtual ~RenderLayer() = default;

    virtual void OnUpdate(BlueMarble::TimeStep ts) override;

private:
    std::shared_ptr<BlueMarble::GameCamera> oCamera;
    std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>> oEntities;
};
