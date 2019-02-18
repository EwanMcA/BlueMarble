#pragma once

#include "core.h"

namespace BlueMarble {

	class BLUEMARBLE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

