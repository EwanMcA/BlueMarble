
#include <BlueMarble.h>

#include "imgui/imgui.h"
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
        oTerrain.Init(64, 64, 0.1f, glm::vec3{ -3.2f, -3.2f, 0.0f });
        oTerrain.ResetHeightMap(BlueMarble::BMPHeightMap("heightmap.bmp"));
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

                glm::vec3 rayDirection = oCamera.CreateRay(normMouseX, normMouseY, 1.0f);
                // World coords at the terrain are camera position + (rayDirection * distance to terrain) 
                glm::vec3 world = oCamera.GetPosition() +
                    rayDirection * abs(oCamera.GetPosition().z / rayDirection.z);

                float relativeX = ((world.x + oTerrain.GetXWidth() / 2.0f) / oTerrain.GetXWidth());
                float relativeY = ((world.y + oTerrain.GetYWidth() / 2.0f) / oTerrain.GetYWidth());

                oTerrain.AddHeight(relativeX * oTerrain.GetXCount(),
                                   relativeY * oTerrain.GetYCount(),
                                   oTerrainModAmount, oTerrainModRadius);
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
            ImGui::SetWindowSize(ImVec2(300, 250), ImGuiCond_FirstUseEver);
            ImGui::Text("Texture Cutoff Heights");
            ImGui::DragFloatRange2("Water", &oTerrainCutoffs.r, &oTerrainCutoffs.g, 0.001f);
            ImGui::DragFloatRange2("Sand", &oTerrainCutoffs.g, &oTerrainCutoffs.b, 0.001f);
            ImGui::DragFloatRange2("Grass", &oTerrainCutoffs.b, &oTerrainCutoffs.a, 0.001f);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("Terrain Modification");
            ImGui::InputFloat("Change", &oTerrainModAmount, 0.001f);
            ImGui::InputFloat("Radius", &oTerrainModRadius, 1);
            ImGui::InputFloat("Height Scale", &oTerrainHeightScale, 0.01f);
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

    BlueMarble::GameCamera oCamera;

    float oCameraMoveSpeed = 5.0f;
    float oTerrainModAmount = 0.05f;
    float oTerrainModRadius = 3.0f;
    float oTerrainHeightScale = 0.25f;

    glm::vec4 oTerrainCutoffs = { 0.0f, 0.015f, 0.025f, 0.2f };
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