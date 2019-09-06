#pragma once

#include <BlueMarble.h>

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define X_VERTICES 128
#define Y_VERTICES 128

using BlueMarble::COMPONENT_TYPE;

class GameLayer : public BlueMarble::Layer
{
public:

    enum GAME_MODE {
        EDITING = 0,
        PLAYING = 1
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

    GameLayer(std::shared_ptr<BlueMarble::GameCamera> camera,
        std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>> entities);

    virtual ~GameLayer() = default;
        
    virtual void OnUpdate(BlueMarble::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(BlueMarble::Event& event) override;

    void CreateTerrain();
    void UpdateTerrain(BlueMarble::TimeStep ts);
    void RenderEditorUI();
    bool OnMouseScrollEvent(BlueMarble::MouseScrollEvent& event);

private:
    std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>> oEntities;

    BlueMarble::Ref<BlueMarble::Terrain> oTerrain;
    std::string oHeightmapFilename;

    std::shared_ptr<BlueMarble::GameCamera> oCamera;

    float oCameraMoveSpeed = 5.0f;
    float oTerrainModAmount = 1.0f;
    float oTerrainModRadius = 3.0f;
    float oTerrainHeightScale = 0.5f;

    glm::vec4 oTerrainCutoffs = { 0.0f, 0.015f, 0.03f, 0.3f };
    BlueMarble::Ref<std::vector<float>> oMoisture;
    BlueMarble::Ref<std::vector<float>> oHeat;

    GAME_MODE oMode{ EDITING };
    DATA_LAYER oEditLayer{ HEIGHT };
    EDIT_MODE oEditMode{ ADD };
};
