#include "sandbox.h"
#include "GameLayer.h"
#include "RenderLayer.h"

class Sandbox : public BlueMarble::Application
{
public:
	Sandbox() 
        : oCamera(glm::radians(45.0f),
                  (float)BlueMarble::Application::Get().GetWindow().GetWidth() /
                  (float)BlueMarble::Application::Get().GetWindow().GetHeight(),
                  0.1f,
                  100.0f)
	{
        PushLayer(new GameLayer(std::shared_ptr<BlueMarble::GameCamera>(&oCamera), 
                                std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>>(&oEntities)));
        PushLayer(new RenderLayer(std::shared_ptr<BlueMarble::GameCamera>(&oCamera), 
                                  std::shared_ptr<std::vector<BlueMarble::Ref<BlueMarble::Entity>>>(&oEntities)));
	}  

    ~Sandbox() = default;

private:
    BlueMarble::GameCamera oCamera;
    std::vector<BlueMarble::Ref<BlueMarble::Entity>> oEntities;
};

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}