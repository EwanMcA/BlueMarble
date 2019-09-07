#pragma once

#include <BlueMarble.h>

class MapLayer : public BlueMarble::Layer 
{
public:
    enum MODE {
        PLAYING = 0,
        EDITING = 1
    };

    enum DATA_LAYER {
        HEIGHT = 0,
        MOISTURE = 1,
        HEAT = 2
    };

    enum EDIT_MODE {
        ADD = 0,
        SUBTRACT = 1,
        SMOOTH = 2
    };

    MapLayer(const uint32_t xCount,
             const uint32_t yCount,
             std::shared_ptr<BlueMarble::GameCamera> camera,
             std::shared_ptr<BlueMarble::EntityComponentSystem> ecs)
        : Layer("MapLayer"), oXCount{ xCount }, oYCount{ yCount }, oCamera(camera), oECS(ecs)
    {}

    virtual ~MapLayer() = default;

    virtual void OnAttach() override;
    virtual void OnUpdate(BlueMarble::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(BlueMarble::Event& event) override;

    void CreateTerrain();
    void UpdateTerrain(BlueMarble::TimeStep ts);
    void RenderEditorUI();

private:
    const uint32_t oXCount;
    const uint32_t oYCount;

    std::shared_ptr<BlueMarble::GameCamera> oCamera;
    std::vector<BlueMarble::Ref<BlueMarble::Entity>> oEntities;
    std::shared_ptr<BlueMarble::EntityComponentSystem> oECS;

    BlueMarble::Ref<BlueMarble::Terrain> oTerrain;
    std::string oHeightmapFilename;

    glm::vec4 oTerrainCutoffs{ 0.0f, 0.015f, 0.03f, 0.3f };
    float oTerrainHeightScale = 0.5f;
    float oTerrainModAmount = 1.0f;
    float oTerrainModRadius = 3.0f;

    BlueMarble::Ref<std::vector<float>> oMoisture;
    BlueMarble::Ref<std::vector<float>> oHeat;

    DATA_LAYER oEditLayer{ HEIGHT };
    EDIT_MODE oEditMode{ ADD };
    MODE oMode{ PLAYING };
};