
#include "BlueMarble.h"

class ExampleLayer : public BlueMarble::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
		//BM_INFO("ExampleLayer::Update");
	}

	void OnEvent(BlueMarble::Event& event) override
	{
		//BM_TRACE("{0}, event");
	}
};

class Sandbox : public BlueMarble::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushLayer(new BlueMarble::RenderLayer());
	}

	~Sandbox()
	{

	}

};

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}