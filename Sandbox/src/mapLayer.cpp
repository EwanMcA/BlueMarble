#include "mapLayer.h"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void MapLayer::OnAttach()
{
    CreateTerrain();
}

void MapLayer::CreateTerrain()
{
    oTerrain = std::make_shared<BlueMarble::Terrain>();
    oTerrain->Init(oXCount, oYCount, 0.05f, glm::vec3{ -3.2f, -3.2f, 0.0f });
    oTerrain->ResetHeightMap(BlueMarble::BMPHeightMap("heightmap.bmp"));
    oTerrain->SetHeightScale(oTerrainHeightScale);

    // Terrain Data Layers
    oMoisture = std::make_shared<std::vector<float>>(oXCount * oYCount, 0.0f);
    oHeat = std::make_shared<std::vector<float>>(oXCount * oYCount, 0.0f);
    oTerrain->AddDataLayer(oMoisture);
    oTerrain->AddDataLayer(oHeat);

    oTerrain->SetTexCoordCallback([](int x, int y) -> std::pair<float, float> {
        float xTex = (x % 2 == 0) ? 0.5f : 1.0f;
        float yTex = (y % 2 == 0) ? 0.5f : 1.0f;
        return { xTex, yTex };
    });

    // Set up oTerrain material //
    BlueMarble::Ref<BlueMarble::Shader> shader =
        BlueMarble::Ref<BlueMarble::Shader>(BlueMarble::Shader::Create("assets/shaders/terrain.glsl"));
    BlueMarble::Ref<BlueMarble::Material> material =
        BlueMarble::Ref<BlueMarble::Material>(BlueMarble::Material::Create(shader));

    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/water.png"));
    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/sand.png"));
    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/grass.png"));
    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/snow.png"));

    material->SetFloat4("uTextureCutoffs", oTerrainCutoffs);
    material->SetBool("uDisplayOverlay", false);

    oTerrain->Load(material);
    /////////////////////////////

    oEntities.push_back(oTerrain);
}


void MapLayer::OnUpdate(BlueMarble::TimeStep ts)
{
    if (oMode == EDITING)
        UpdateTerrain(ts);

    oECS->Update(ts, oEntities);
}

void MapLayer::UpdateTerrain(BlueMarble::TimeStep ts)
{
    oTerrain->GetComponent<BlueMarble::MaterialComponent>()->
        oMaterial->SetFloat4("uTextureCutoffs", oTerrainCutoffs);

    unsigned int width = BlueMarble::Application::Get().GetWindow().GetWidth();
    unsigned int height = BlueMarble::Application::Get().GetWindow().GetHeight();

    if (BlueMarble::Input::IsMouseButtonPressed(0))
    {
        float normMouseX = BlueMarble::Input::GetMouseX() / (width * 0.5f) - 1.0f;
        float normMouseY = BlueMarble::Input::GetMouseY() / (height * 0.5f) - 1.0f;
        float normMouseZ = 2 * BlueMarble::Input::GetMouseZ() - 1.0f;

        glm::vec3 world = oCamera->GetWorldCoords(normMouseX, normMouseY, normMouseZ);

        float xRatio = (world.x - oTerrain->GetPosition().x) / oTerrain->GetXWidth();
        float yRatio = (world.y - oTerrain->GetPosition().y) / oTerrain->GetYWidth();

        if (0.0f <= xRatio && xRatio <= 1.0f && 0.0f <= yRatio && yRatio <= 1.0f)
        {
            int x = (int)(xRatio * oTerrain->GetXCount());
            int y = (int)(yRatio * oTerrain->GetYCount());
            if (oEditMode == SMOOTH)
            {
                oTerrain->LayerSmooth(oEditLayer, x, y, oTerrainModRadius);
            }
            else
            {
                int sign = (oEditMode == ADD) ? 1 : -1;
                oTerrain->LayerAdd(oEditLayer, x, y, sign * oTerrainModAmount * ts, oTerrainModRadius);
            }
        }
    }
    oTerrain->SetHeightScale(oTerrainHeightScale);
}

void MapLayer::OnImGuiRender()
{

    ImGui::Begin("Mode");

    ImGui::RadioButton("Map Editor", (int*)&oMode, EDITING);
    ImGui::RadioButton("Gameplay", (int*)&oMode, PLAYING);

    ImGui::End();

    if (oMode == EDITING)
        RenderEditorUI();
}

void MapLayer::RenderEditorUI()
{
    ImGui::Begin("Terrain");
    ImGui::SetWindowSize(ImVec2(350, 375), ImGuiCond_FirstUseEver);
    ImGui::Text("Texture Cutoff Heights");
    ImGui::NewLine();
    ImGui::DragFloatRange2("Water", &oTerrainCutoffs.r, &oTerrainCutoffs.g, 0.001f);
    ImGui::DragFloatRange2("Sand", &oTerrainCutoffs.g, &oTerrainCutoffs.b, 0.001f);
    ImGui::DragFloatRange2("Grass", &oTerrainCutoffs.b, &oTerrainCutoffs.a, 0.001f);

    ImGui::NewLine(); ImGui::Separator();

    ImGui::Text("Terrain Modification");
    ImGui::NewLine();

    ImGui::BeginGroup();
    if (ImGui::RadioButton("Height", (int*)&oEditLayer, HEIGHT))
    {
        oTerrain->GetComponent<BlueMarble::MaterialComponent>()->oMaterial->SetBool("uDisplayOverlay", false);
    }
    else if (ImGui::RadioButton("Moisture", (int*)&oEditLayer, MOISTURE))
    {
        oTerrain->SetOverlayCallback([this](int x, int y) -> float { return (*oMoisture)[x + y * oXCount]; });
        oTerrain->RefreshOverlay();
        oTerrain->GetComponent<BlueMarble::MaterialComponent>()->oMaterial->SetBool("uDisplayOverlay", true);
    }
    if (ImGui::RadioButton("Heat", (int*)&oEditLayer, HEAT))
    {
        oTerrain->SetOverlayCallback([this](int x, int y) -> float { return (*oHeat)[x + y * oXCount]; });
        oTerrain->RefreshOverlay();
        oTerrain->GetComponent<BlueMarble::MaterialComponent>()->oMaterial->SetBool("uDisplayOverlay", true);
    }

    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::RadioButton("Add", (int*)&oEditMode, ADD);
    ImGui::RadioButton("Subtract", (int*)&oEditMode, SUBTRACT);
    ImGui::RadioButton("Smooth", (int*)&oEditMode, SMOOTH);
    ImGui::EndGroup();
    ImGui::NewLine();

    if (oEditMode == ADD || oEditMode == SUBTRACT)
        ImGui::InputFloat("Change", &oTerrainModAmount, 0.5f);
    ImGui::InputFloat("Radius", &oTerrainModRadius, 1);
    if (ImGui::InputFloat("Height Scale", &oTerrainHeightScale, 0.01f))
        oTerrain->RefreshVertices();

    ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

    ImGui::InputText("heightmap file (bmp)", &oHeightmapFilename);
    if (ImGui::Button("Reset")) {
        if (!oHeightmapFilename.empty()) {
            BlueMarble::BMPHeightMap hm(oHeightmapFilename);
            oTerrain->ResetHeightMap(hm);
        }
        else {
            oTerrain->ResetHeightMap();
        }
        oTerrain->RefreshVertices();
    }
    ImGui::End();
}

void MapLayer::OnEvent(BlueMarble::Event & event)
{

}
