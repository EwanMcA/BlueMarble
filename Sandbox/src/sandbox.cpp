
#include <BlueMarble.h>

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define X_VERTICES 128
#define Y_VERTICES 128

class GameLayer : public BlueMarble::Layer
{
public:

    typedef enum GAME_MODE {
        EDITING = 0,
        PLAYING = 1
    };

    typedef enum EDIT_MODE {
        ADD = 0,
        SUBTRACT = 1,
        SMOOTH = 2
    };

	GameLayer()
        : Layer("Example"), 
          oCamera(glm::radians(45.0f),
                  (float)BlueMarble::Application::Get().GetWindow().GetWidth() / 
                  (float)BlueMarble::Application::Get().GetWindow().GetHeight(), 
                   0.1f, 
                   100.0f) 
	{
        oCamera.Translate(0.0f, -3.2f);
        oTerrain.Init(X_VERTICES, Y_VERTICES, 0.05f, glm::vec3{ -3.2f, -3.2f, 0.0f });
        oTerrain.ResetHeightMap(BlueMarble::BMPHeightMap("heightmap.bmp"));
        oTerrain.SetHeightScale(oTerrainHeightScale);
        oMoisture.resize(X_VERTICES * Y_VERTICES, 0.0f);
        
        oTerrain.SetTexCoordCallback([](int x, int y) -> std::pair<float, float> {
            float xTex = (x % 2 == 0) ? 0.5f : 1.0f;
            float yTex = (y % 2 == 0) ? 0.5f : 1.0f;
            return { xTex, yTex };
        });

        oTerrain.SetVertexStatsCallback([this](int x, int y) -> std::tuple<float, float> {
            return { oMoisture[y * X_VERTICES + x], 0.0f };
        });

        oTerrain.Load();

        BlueMarble::Ref<BlueMarble::Texture2D> texture;
        texture = BlueMarble::Texture2D::Create("assets/textures/water.png");
        oTextures.push_back(texture);
        texture = BlueMarble::Texture2D::Create("assets/textures/sand.png");
        oTextures.push_back(texture);
        texture = BlueMarble::Texture2D::Create("assets/textures/grass.png");
        oTextures.push_back(texture);
        texture = BlueMarble::Texture2D::Create("assets/textures/snow.png");
        oTextures.push_back(texture);

    }       

    void UpdateTerrain(BlueMarble::TimeStep ts)
    {
        unsigned int width = BlueMarble::Application::Get().GetWindow().GetWidth();
        unsigned int height = BlueMarble::Application::Get().GetWindow().GetHeight();

        if (BlueMarble::Input::IsMouseButtonPressed(0))
        {
            float normMouseX = BlueMarble::Input::GetMouseX() / (width * 0.5f) - 1.0f;
            float normMouseY = BlueMarble::Input::GetMouseY() / (height * 0.5f) - 1.0f;
            float normMouseZ = 2 * BlueMarble::Input::GetMouseZ() - 1.0f;

            glm::vec3 world = oCamera.GetWorldCoords(normMouseX, normMouseY, normMouseZ);

            float xRatio = (world.x - oTerrain.getPosition().x) / oTerrain.GetXWidth();
            float yRatio = (world.y - oTerrain.getPosition().y) / oTerrain.GetYWidth();
            if (oEditMode == SMOOTH) 
            {
                oTerrain.SmoothHeight(xRatio * oTerrain.GetXCount(),
                                      yRatio * oTerrain.GetYCount(),
                                      oTerrainModRadius);
            }
            else
            {
                int sign = (oEditMode == ADD) ? 1 : -1;
                oTerrain.AddHeight(xRatio * oTerrain.GetXCount(),
                                   yRatio * oTerrain.GetYCount(),
                                   sign * oTerrainModAmount * ts, 
                                   oTerrainModRadius);
            }

        }
        oTerrain.SetHeightScale(oTerrainHeightScale);
    }

    void OnUpdate(BlueMarble::TimeStep ts) override
    {
        if (BlueMarble::Input::IsKeyPressed(BM_KEY_LEFT) ||
            BlueMarble::Input::IsKeyPressed(BM_KEY_A))
        {
            oCamera.Translate(-oCameraMoveSpeed * ts, 0.0f);
        }
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_RIGHT) ||
            BlueMarble::Input::IsKeyPressed(BM_KEY_D))
        {
            oCamera.Translate(oCameraMoveSpeed * ts, 0.0f);
        }

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_UP) ||
            BlueMarble::Input::IsKeyPressed(BM_KEY_W))
        {
            oCamera.Translate(0.0f, oCameraMoveSpeed * ts);
        }
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_DOWN) ||
            BlueMarble::Input::IsKeyPressed(BM_KEY_S))
        {
            oCamera.Translate(0.0f, -oCameraMoveSpeed * ts);
        }

        if (oMode == EDITING)
        {
            UpdateTerrain(ts);
        }
        
        BlueMarble::RenderCommand::SetClearColor({ 0.4f, 0.6f, 1.0f, 1 });
        BlueMarble::RenderCommand::Clear();

        BlueMarble::Renderer::BeginScene(oCamera);

        oTerrain.Draw(oTextures, oTerrainCutoffs);

        BlueMarble::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
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

    void RenderEditorUI()
    {
        ImGui::Begin("Terrain");
        ImGui::SetWindowSize(ImVec2(350, 375), ImGuiCond_FirstUseEver);
        ImGui::Text("Texture Cutoff Heights");
        ImGui::NewLine();
        ImGui::DragFloatRange2("Water", &oTerrainCutoffs.r, &oTerrainCutoffs.g, 0.001f);
        ImGui::DragFloatRange2("Sand", &oTerrainCutoffs.g, &oTerrainCutoffs.b, 0.001f);
        ImGui::DragFloatRange2("Grass", &oTerrainCutoffs.b, &oTerrainCutoffs.a, 0.001f);

        ImGui::Separator();

        ImGui::Text("Terrain Modification");
        ImGui::NewLine();

        ImGui::RadioButton("Add", (int*)&oEditMode, ADD);
        ImGui::RadioButton("Subtract", (int*)&oEditMode, SUBTRACT);
        ImGui::RadioButton("Smooth", (int*)&oEditMode, SMOOTH);
        ImGui::NewLine();

        if (oEditMode == 1 || oEditMode == 2)
            ImGui::InputFloat("Change", &oTerrainModAmount, 0.5f);
        ImGui::InputFloat("Radius", &oTerrainModRadius, 1);
        ImGui::InputFloat("Height Scale", &oTerrainHeightScale, 0.01f);

        ImGui::Separator();

        ImGui::NewLine();
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

    bool OnMouseScrollEvent(BlueMarble::MouseScrollEvent& event)
    {
        oCamera.Zoom(event.GetYOffset());
        return true;
    }

	void OnEvent(BlueMarble::Event& event) override
	{
        BlueMarble::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<BlueMarble::MouseScrollEvent>(BM_BIND_EVENT_FN(GameLayer::OnMouseScrollEvent));
	}

private:
    BlueMarble::Terrain oTerrain;
    std::string oHeightmapFilename;
    std::vector<BlueMarble::Ref<BlueMarble::Texture2D>> oTextures;

    BlueMarble::GameCamera oCamera;

    float oCameraMoveSpeed = 5.0f;
    float oTerrainModAmount = 1.0f;
    float oTerrainModRadius = 3.0f;
    float oTerrainHeightScale = 0.5f;

    glm::vec4 oTerrainCutoffs = { 0.0f, 0.015f, 0.03f, 0.3f };
    std::vector<float> oMoisture;

    GAME_MODE oMode{ EDITING };
    EDIT_MODE oEditMode{ ADD };
};

class Sandbox : public BlueMarble::Application
{
public:
	Sandbox() 
	{
		PushLayer(new GameLayer());
	}  

	~Sandbox()
	{

	}

};

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}