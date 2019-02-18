#pragma once

#ifdef BM_PLATFORM_WINDOWS

extern BlueMarble::Application* BlueMarble::CreateApplication();

int main(int argc, char** argv)
{
	auto app = BlueMarble::CreateApplication();
	app->Run();
	delete app;
}

#endif // BM_PLATFORM_WINDOWS

