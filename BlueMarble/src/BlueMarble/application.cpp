#include "bmpch.h"
#include "application.h"

#include "BlueMarble/Events/event.h"
#include "BlueMarble/log.h"

#include <Glad/glad.h>

#include "input.h"

namespace BlueMarble {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::cInstance = nullptr;

	Application::Application()
	{
		BM_CORE_ASSERT(!cInstance, "Application already exists");
		cInstance = this;
		oWindow = std::unique_ptr<Window>(Window::Create());
		oWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

		oImGuiLayer = new ImGuiLayer();
		PushOverlay(oImGuiLayer);

        glGenVertexArrays(1, &oVertexArray);
        glBindVertexArray(oVertexArray);

        glGenBuffers(1, &oVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, oVertexBuffer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &oIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oIndexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{
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

		BM_CORE_TRACE("{0}", e);

		for (auto it = oLayerStack.end(); it != oLayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}


	void Application::Run() 
	{
		while (oRunning) {

            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(oVertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : oLayerStack)
				layer->OnUpdate();

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
