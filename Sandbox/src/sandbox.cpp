
#include <BlueMarble.h>

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public BlueMarble::Layer
{
public:
	ExampleLayer()
        : Layer("Example"), 
          oCamera(0.77f, 
                  (float)BlueMarble::Application::Get().GetWindow().GetWidth() / 
                  (float)BlueMarble::Application::Get().GetWindow().GetHeight(), 
                   0.1f, 
                   10.0f), 
        oCameraPosition({ 0.0f, 0.0f, 1.0f })
	{
        oTerrain.Init(64, 64, 0.1f);
        oTerrain.ResetHeightMap(BlueMarble::BMPHeightMap("heightmap.bmp"));
        oTerrain.Load();
    }

	void OnUpdate(BlueMarble::TimeStep ts) override
	{
        //BM_TRACE("Delta time: {0} ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
        if (BlueMarble::Input::IsKeyPressed(BM_KEY_LEFT))
            oCameraPosition.x -= oCameraMoveSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_RIGHT))
            oCameraPosition.x += oCameraMoveSpeed * ts;
        
        if (BlueMarble::Input::IsKeyPressed(BM_KEY_UP))
            oCameraPosition.y += oCameraMoveSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_DOWN))
            oCameraPosition.y -= oCameraMoveSpeed * ts;

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_W)) {
            if (oCameraPosition.z > 0.5f) {
                oCameraPosition.z -= oCameraMoveSpeed * ts;
                if (oCameraPosition.z < 2.5f) {
                    oCameraRotation.x += oCameraRotationSpeed * ts;
                    oCameraRotation.x = std::min(80.0f, oCameraRotation.x);
                }
            }
        }
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_S)) {
            //if (oCameraPosition.z < 4.5f) {
                oCameraPosition.z += oCameraMoveSpeed * ts;
                oCameraRotation.x -= oCameraRotationSpeed * ts;
                oCameraRotation.x = std::max(0.0f, oCameraRotation.x);
            //}
        }

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_A))
            oCameraRotation.z += oCameraRotationSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_D))
            oCameraRotation.z -= oCameraRotationSpeed * ts;

        if (BlueMarble::Input::IsMouseButtonPressed(0)) 
        {
            oTerrain.AddHeight(20, 20, 0.01f, 5);
        }

        BlueMarble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        BlueMarble::RenderCommand::Clear();

        oCamera.SetPosition(oCameraPosition);
        oCamera.SetRotation(oCameraRotation);

        BlueMarble::Renderer::BeginScene(oCamera);

        oTerrain.Draw();
        /*std::dynamic_pointer_cast<BlueMarble::OpenGLShader>(oFlatShader)->Bind();
        std::dynamic_pointer_cast<BlueMarble::OpenGLShader>(oFlatShader)->UploadUniformFloat3("uColor", oSquareColor);*/

        BlueMarble::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        //ImGui::ColorEdit3("Square Color", glm::value_ptr(oSquareColor));
        ImGui::End();
    }

	void OnEvent(BlueMarble::Event& event) override
	{
		//BM_TRACE("{0}, event");
        /*BlueMarble::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<BlueMarble::KeyPressEvent>(BM_BIND_EVENT_FN(ExampleLayer::OnKeyPressEvent));*/
	}

private:
    BlueMarble::Terrain oTerrain;

    BlueMarble::PerspectiveCamera oCamera;
    glm::vec3 oCameraPosition;
    glm::vec3 oCameraRotation = glm::vec3(0.0f);
    float oCameraMoveSpeed = 2.0f;
    float oCameraRotationSpeed = 90.0f;

    //glm::vec3 oSquareColor = { 0.2f, 0.3f, 0.8f };
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