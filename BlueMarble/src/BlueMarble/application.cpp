#include "bmpch.h"
#include "application.h"

#include "BlueMarble/Events/event.h"
#include "BlueMarble/log.h"

#include "BlueMarble/Renderer/renderer.h"

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

        oVertexArray.reset(VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = {
            {ShaderDataType::Float3, "aPosition" },
            {ShaderDataType::Float4, "aColor" },
        };

        vertexBuffer->SetLayout(layout);
        oVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        oVertexArray->SetIndexBuffer(indexBuffer);

        oSquareVA.reset(VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({{ ShaderDataType::Float3, "aPosition" }});
        oSquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        oSquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 aPosition;
            layout(location = 1) in vec4 aColor;

            out vec3 vPosition;
            out vec4 vColor;

            void main()
            {
                vPosition = aPosition;
                vColor = aColor;
                gl_Position = vec4(aPosition, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 vPosition;
            in vec4 vColor;

            void main()
            {
                color = vec4(vPosition * 0.5 + 0.5, 1.0);
                color = vColor;
            }
        )";
        oShader.reset(new Shader(vertexSrc, fragmentSrc));

        std::string blueVertexShaderSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 aPosition;

            out vec3 vPosition;

            void main()
            {
                vPosition = aPosition;
                gl_Position = vec4(aPosition, 1.0);
            }
        )";

        std::string blueFragmentShaderSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 vPosition;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        oBlueShader.reset(new Shader(blueVertexShaderSrc, blueFragmentShaderSrc));
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
		while (oRunning) {

            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
            RenderCommand::Clear();

            Renderer::BeginScene();

            oBlueShader->Bind();
            Renderer::Submit(oSquareVA);
            
            oShader->Bind();
            Renderer::Submit(oVertexArray);

            Renderer::EndScene();

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
