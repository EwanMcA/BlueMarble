#pragma once

#include "core.h"
#include "Window.h"
#include "BlueMarble/layerStack.h"
#include "BlueMarble/Events/event.h"
#include "BlueMarble/Events/applicationEvent.h"

#include "BlueMarble/ImGui/imGuiLayer.h"

#include "BlueMarble/Renderer/VertexArray.h"
#include "BlueMarble/Renderer/Shader.h"
#include "BlueMarble/Renderer/Buffer.h"

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

		inline static Application& Get() { return *cInstance; }
		inline Window& GetWindow() { return *oWindow;  }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::shared_ptr<Window> oWindow;
		ImGuiLayer* oImGuiLayer;
		
		bool oRunning = true;

		LayerStack oLayerStack; 

        std::shared_ptr<Shader> oShader;
        std::shared_ptr<VertexArray> oVertexArray;

        std::shared_ptr<Shader> oBlueShader;
        std::shared_ptr<VertexArray> oSquareVA;

		static Application* cInstance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

