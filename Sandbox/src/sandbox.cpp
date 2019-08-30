
#include <BlueMarble.h>

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public BlueMarble::Layer
{
public:
	ExampleLayer()
        : Layer("Example"), 
          oCamera(glm::radians(45.0f),
                  (float)BlueMarble::Application::Get().GetWindow().GetWidth() / 
                  (float)BlueMarble::Application::Get().GetWindow().GetHeight(), 
                   0.1f, 
                   100.0f) 
	{
        oTerrain.Init(128, 128, 0.05f, glm::vec3{ -3.2f, -3.2f, 0.0f });
        oTerrain.ResetHeightMap(BlueMarble::BMPHeightMap("heightmap.bmp"));
        oTerrain.SetHeightScale(oTerrainHeightScale);
        
        oTerrain.SetTexCoordCallback([](int x, int y) -> std::pair<float, float> {
            float xTex = (x % 2 == 0) ? 0.5f : 1.0f;
            float yTex = (y % 2 == 0) ? 0.5f : 1.0f;
            return { xTex, yTex };
        });

        oTerrain.SetVertexStatsCallback([this](int x, int y) -> std::tuple<float, float> {
            return { 1.0f, 0.0f };
        });

        oTerrain.Load();
        oCamera.Translate(0.0f, -3.2f);
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

        // Demo manipulating terrain height
        {
            unsigned int width = BlueMarble::Application::Get().GetWindow().GetWidth();
            unsigned int height = BlueMarble::Application::Get().GetWindow().GetHeight();

            if (BlueMarble::Input::IsMouseButtonPressed(0))
            {
                float normMouseX = BlueMarble::Input::GetMouseX() / (width * 0.5f) - 1.0f;
                float normMouseY = BlueMarble::Input::GetMouseY() / (height * 0.5f) - 1.0f;
                float normMouseZ = 2 * BlueMarble::Input::GetMouseZ() - 1.0f;
                
                glm::vec3 world = oCamera.GetWorldCoords(normMouseX, normMouseY, normMouseZ);

                float xRatio = (world.x - oTerrain.getPosition().x)  / oTerrain.GetXWidth();
                float yRatio = (world.y - oTerrain.getPosition().y) / oTerrain.GetYWidth();

                oTerrain.AddHeight(xRatio * oTerrain.GetXCount(),
                                   yRatio * oTerrain.GetYCount(),
                                   oTerrainModAmount * ts, oTerrainModRadius);
            }
        }

        oTerrain.SetHeightScale(oTerrainHeightScale);
        
        BlueMarble::RenderCommand::SetClearColor({ 0.4f, 0.6f, 1.0f, 1 });
        BlueMarble::RenderCommand::Clear();

        BlueMarble::Renderer::BeginScene(oCamera);

        oTerrain.Draw(oTerrainCutoffs);

        BlueMarble::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Terrain", nullptr);
            ImGui::SetWindowSize(ImVec2(300, 325), ImGuiCond_FirstUseEver);
            ImGui::Text("Texture Cutoff Heights");
            ImGui::NewLine();
            ImGui::DragFloatRange2("Water", &oTerrainCutoffs.r, &oTerrainCutoffs.g, 0.001f);
            ImGui::DragFloatRange2("Sand", &oTerrainCutoffs.g, &oTerrainCutoffs.b, 0.001f);
            ImGui::DragFloatRange2("Grass", &oTerrainCutoffs.b, &oTerrainCutoffs.a, 0.001f);
            
            ImGui::Separator();

            ImGui::Text("Terrain Modification");
            ImGui::NewLine();
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
        dispatcher.Dispatch<BlueMarble::MouseScrollEvent>(BM_BIND_EVENT_FN(ExampleLayer::OnMouseScrollEvent));
	}

private:
    BlueMarble::Terrain oTerrain;
    std::string oHeightmapFilename;

    BlueMarble::GameCamera oCamera;

    float oCameraMoveSpeed = 5.0f;
    float oTerrainModAmount = 1.0f;
    float oTerrainModRadius = 3.0f;
    float oTerrainHeightScale = 0.5f;

    glm::vec4 oTerrainCutoffs = { 0.0f, 0.015f, 0.03f, 0.3f };
};

class Sandbox : public BlueMarble::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}  

	~Sandbox()
	{

	}

};

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}