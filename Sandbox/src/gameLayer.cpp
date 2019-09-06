#include "gameLayer.h"

GameLayer::GameLayer(std::shared_ptr<BlueMarble::GameCamera> camera,
    std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>> entities)
    : Layer("GameLayer"),
    oCamera(camera), oEntities(entities)
{
    oCamera->Translate(0.0f, -3.2f);
    oTerrain.Init(X_VERTICES, Y_VERTICES, 0.05f, glm::vec3{ -3.2f, -3.2f, 0.0f });
    oTerrain.ResetHeightMap(BlueMarble::BMPHeightMap("heightmap.bmp"));
    oTerrain.SetHeightScale(oTerrainHeightScale);

    // Terrain Data Layers
    oMoisture.resize(X_VERTICES * Y_VERTICES, 0.0f);
    oHeat.resize(X_VERTICES * Y_VERTICES, 0.0f);
    oTerrain.AddDataLayer(BlueMarble::Ref<std::vector<float>>(&oMoisture));
    oTerrain.AddDataLayer(BlueMarble::Ref<std::vector<float>>(&oHeat));

    oTerrain.SetTexCoordCallback([](int x, int y) -> std::pair<float, float> {
        float xTex = (x % 2 == 0) ? 0.5f : 1.0f;
        float yTex = (y % 2 == 0) ? 0.5f : 1.0f;
        return { xTex, yTex };
    });

    // Set up terrain material //
    BlueMarble::Ref<BlueMarble::Shader> shader =
        BlueMarble::Ref<BlueMarble::Shader>(BlueMarble::Shader::Create("assets/shaders/terrain.glsl"));
    BlueMarble::Ref<BlueMarble::Material> material =
        BlueMarble::Ref<BlueMarble::Material>(BlueMarble::Material::Create(shader));

    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/water.png"));
    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/sand.png"));
    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/grass.png"));
    material->AddTexture2D(BlueMarble::Texture2D::Create("assets/textures/snow.png"));

    material->SetFloat4("uTextureCutoffs", oTerrainCutoffs);

    oTerrain.Load(material);
    oEntities->push_back(BlueMarble::Ref<BlueMarble::Entity>(&oTerrain));
    /////////////////////////////
}

void GameLayer::UpdateTerrain(BlueMarble::TimeStep ts)
{
    oTerrain.GetComponent<BlueMarble::MaterialComponent>(COMPONENT_TYPE::MATERIAL)->
        oMaterial->SetFloat4("uTextureCutoffs", oTerrainCutoffs);

    unsigned int width = BlueMarble::Application::Get().GetWindow().GetWidth();
    unsigned int height = BlueMarble::Application::Get().GetWindow().GetHeight();

    if (BlueMarble::Input::IsMouseButtonPressed(0))
    {
        float normMouseX = BlueMarble::Input::GetMouseX() / (width * 0.5f) - 1.0f;
        float normMouseY = BlueMarble::Input::GetMouseY() / (height * 0.5f) - 1.0f;
        float normMouseZ = 2 * BlueMarble::Input::GetMouseZ() - 1.0f;

        glm::vec3 world = oCamera->GetWorldCoords(normMouseX, normMouseY, normMouseZ);

        float xRatio = (world.x - oTerrain.GetPosition().x) / oTerrain.GetXWidth();
        float yRatio = (world.y - oTerrain.GetPosition().y) / oTerrain.GetYWidth();

        if (0.0f <= xRatio && xRatio <= 1.0f && 0.0f <= yRatio && yRatio <= 1.0f)
        {
            int x = (int)(xRatio * oTerrain.GetXCount());
            int y = (int)(yRatio * oTerrain.GetYCount());
            if (oEditMode == SMOOTH)
            {
                oTerrain.LayerSmooth(oEditLayer, x, y, oTerrainModRadius);
            }
            else
            {
                int sign = (oEditMode == ADD) ? 1 : -1;
                oTerrain.LayerAdd(oEditLayer, x, y, sign * oTerrainModAmount * ts, oTerrainModRadius);
            }
        }
    }
    oTerrain.SetHeightScale(oTerrainHeightScale);
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

    if (oMode == EDITING)
    {
        UpdateTerrain(ts);
    }
}

void GameLayer::OnImGuiRender() 
{
    ImGui::Begin("Mode");
    ImGui::RadioButton("Terrain Editor", (int*)&oMode, EDITING);
    ImGui::RadioButton("Gameplay", (int*)&oMode, PLAYING);
    ImGui::End();

    if (oMode == EDITING)
    {
        RenderEditorUI();
    }
}

void GameLayer::RenderEditorUI()
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
    ImGui::RadioButton("Height", (int*)&oEditLayer, HEIGHT);
    ImGui::RadioButton("Moisture", (int*)&oEditLayer, MOISTURE);
    ImGui::RadioButton("Heat", (int*)&oEditLayer, HEAT);
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
        oTerrain.RefreshVertices();

    ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

    ImGui::InputText("heightmap file (bmp)", &oHeightmapFilename);
    if (ImGui::Button("Reset")) {
        if (!oHeightmapFilename.empty()) {
            BlueMarble::BMPHeightMap hm(oHeightmapFilename);
            oTerrain.ResetHeightMap(hm);
        }
        else {
            oTerrain.ResetHeightMap();
        }
        oTerrain.RefreshVertices();
    }
    ImGui::End();
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
