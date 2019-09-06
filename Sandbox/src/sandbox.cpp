#include "sandbox.h"

#include <BlueMarble/entryPoint.h>

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}
