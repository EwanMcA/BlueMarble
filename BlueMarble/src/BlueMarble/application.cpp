#include "bmpch.h"
#include "application.h"

#include "BlueMarble/Events/event.h"
#include "BlueMarble/log.h"

#include "BlueMarble/Renderer/renderer.h"

#include "input.h"

#include "GLFW/glfw3.h"

namespace BlueMarble {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::cInstance = nullptr;

    Application::Application()
    {
        BM_CORE_ASSERT(!cInstance, "Application already exists");
        cInstance = this;
        oWindow = std::unique_ptr<Window>(Window::Create());
        oWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        oImGuiLayer = new ImGuiLayer();
        PushOverlay(oImGuiLayer);
	}

	void Application::PushLayer(Layer* layer)
	{
		oLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		oLayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//BM_CORE_TRACE("{0}", e); 

		for (auto it = oLayerStack.end(); it != oLayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() 
	{
		while (oRunning) 
        {
            float time = (float) glfwGetTime(); // Eventually make this Platform::GetTime()
            TimeStep timeStep = time - oLastFrameTime;
            oLastFrameTime = time;

			for (Layer* layer : oLayerStack)
				layer->OnUpdate(timeStep);

			oImGuiLayer->Begin(); 
			for (Layer* layer : oLayerStack)
				layer->OnImGuiRender();
			oImGuiLayer->End();
			 
			oWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		oRunning = false;
		return true;
	}

} // namespace BlueMarble
