#pragma once

#include "core.h"
#include "Window.h"
#include "BlueMarble/layerStack.h"
#include "BlueMarble/Events/event.h"
#include "BlueMarble/Events/applicationEvent.h"

#include "BlueMarble/Core/timeStep.h"

#include "BlueMarble/ImGui/imGuiLayer.h"

namespace BlueMarble {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *oWindow;  }

		inline static Application& Get() { return *cInstance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

    private:
		std::shared_ptr<Window> oWindow;
		ImGuiLayer* oImGuiLayer;
		bool oRunning = true;
		LayerStack oLayerStack; 
        float oLastFrameTime = 0.0f;

    private:
		static Application* cInstance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

