#pragma once

#ifdef BM_PLATFORM_WINDOWS

extern BlueMarble::Application* BlueMarble::CreateApplication();

int main(int argc, char** argv)
{
	BlueMarble::Log::Init();
	BM_CORE_WARN("Init Log");
	BM_INFO("Init Log");

	auto app = BlueMarble::CreateApplication();  
	app->Run();
	delete app;
}

#endif // BM_PLATFORM_WINDOWS

