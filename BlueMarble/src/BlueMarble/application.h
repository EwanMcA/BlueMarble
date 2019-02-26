#pragma once

#include "core.h"
#include "Window.h"
#include "BlueMarble/layerStack.h"
#include "BlueMarble/Events/event.h"
#include "BlueMarble/Events/applicationEvent.h"

namespace BlueMarble {

	class BLUEMARBLE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *cInstance; }
		inline Window& GetWindow() { return *oWindow;  }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> oWindow;
		bool oRunning = true;

		LayerStack oLayerStack; 

		static Application* cInstance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

