
#include "BlueMarble.h"

class Sandbox : public BlueMarble::Application
{
public:
	Sandbox() 
	{

	}

	~Sandbox()
	{

	}

};

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}