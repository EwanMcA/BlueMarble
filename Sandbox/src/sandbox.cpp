
#include <BlueMarble.h>
#include "glm/gtc/matrix_transform.hpp"

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
        oTerrain.Init(20, 20, 0.1f);
        oTerrain.GenerateRandomHeightMap();
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
                if (oCameraPosition.z < 1.5f) {
                    oCameraRotation.x += oCameraRotationSpeed * ts;
                    oCameraRotation.x = std::min(80.0f, oCameraRotation.x);
                }
            }
        }
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_S)) {
            if (oCameraPosition.z < 4.5f) {
                oCameraPosition.z += oCameraMoveSpeed * ts;
                oCameraRotation.x -= oCameraRotationSpeed * ts;
                oCameraRotation.x = std::max(0.0f, oCameraRotation.x);
            }
        }

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_A))
            oCameraRotation.z += oCameraRotationSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_D))
            oCameraRotation.z -= oCameraRotationSpeed * ts;

        BlueMarble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        BlueMarble::RenderCommand::Clear();

        oCamera.SetPosition(oCameraPosition);
        oCamera.SetRotation(oCameraRotation);

        BlueMarble::Renderer::BeginScene(oCamera);

        //glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        //glm::vec3 pos(x * 0.1f, y * 0.1f, 0.0f);
        //glm::mat4 transform = glm::mat4(1.0f) * scale;
        //oFlatShader->UploadUniformFloat4("uColor", glm::vec4(1.0f, 1.0f - (y+x)/60.0f, 1.0f - (x+y)/40.0f, 0.0f));

        oTerrain.Draw();

        BlueMarble::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {

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