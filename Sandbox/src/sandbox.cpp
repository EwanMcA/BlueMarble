
#include <BlueMarble.h>
#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public BlueMarble::Layer
{
public:
	ExampleLayer()
        : Layer("Example"), oCamera(-2.0f, 2.0f, -2.0f, 2.0f), oCameraPosition(0.0f)
	{
        oVertexArray.reset(BlueMarble::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<BlueMarble::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(BlueMarble::VertexBuffer::Create(vertices, sizeof(vertices)));

        BlueMarble::BufferLayout layout = {
            {BlueMarble::ShaderDataType::Float3, "aPosition" },
            {BlueMarble::ShaderDataType::Float4, "aColor" },
        };

        vertexBuffer->SetLayout(layout);
        oVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<BlueMarble::IndexBuffer> indexBuffer;
        indexBuffer.reset(BlueMarble::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        oVertexArray->SetIndexBuffer(indexBuffer);

        oSquareVA.reset(BlueMarble::VertexArray::Create());

        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f,     0.5f, 0.5f,
             0.5f, -0.5f, 0.0f,     1.0f, 0.5f,
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,     0.5f, 1.0f
        };

        std::shared_ptr<BlueMarble::VertexBuffer> squareVB;
        squareVB.reset(BlueMarble::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({ { BlueMarble::ShaderDataType::Float3, "aPosition" },
                              { BlueMarble::ShaderDataType::Float2, "aTexCoord" } });
        oSquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<BlueMarble::IndexBuffer> squareIB;
        squareIB.reset(BlueMarble::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        oSquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 aPosition;
            layout(location = 1) in vec4 aColor;

            uniform mat4 uViewProjection;
            uniform mat4 uTransform;

            out vec3 vPosition;
            out vec4 vColor;

            void main()
            {
                vPosition = aPosition;
                vColor = aColor;
                gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
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
        oShader.reset(new BlueMarble::Shader(vertexSrc, fragmentSrc));

        std::string flatVertexShaderSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 aPosition;
            layout(location = 1) in vec2 aTexCoord;

            uniform mat4 uViewProjection;
            uniform mat4 uTransform;

            out vec3 vPosition;
            out vec2 vTexCoord;

            void main()
            {
                vPosition = aPosition;
                vTexCoord = aTexCoord;
                gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
            }
        )";

        std::string flatFragmentShaderSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 vPosition;
            in vec2 vTexCoord;
            
            uniform sampler2D uTexture1;
            uniform vec4 uColor;

            void main()
            {
                color = texture( uTexture1, vTexCoord ) * uColor;
            }
        )";

        oFlatShader.reset(new BlueMarble::Shader(flatVertexShaderSrc, flatFragmentShaderSrc));

        oTexture.reset(BlueMarble::Texture::Create("grass.png"));
    }

	void OnUpdate(BlueMarble::TimeStep ts) override
	{
        //BM_TRACE("Delta time: {0} ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
        if (BlueMarble::Input::IsKeyPressed(BM_KEY_LEFT))
            oCameraPosition.x -= oCameraMoveSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_RIGHT))
            oCameraPosition.x += oCameraMoveSpeed * ts;
        
        if (BlueMarble::Input::IsKeyPressed(BM_KEY_UP))
            oCameraPosition.y += oCameraMoveSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_DOWN))
            oCameraPosition.y -= oCameraMoveSpeed * ts;

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_A))
            oCameraRotation += oCameraRotationSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_D))
            oCameraRotation -= oCameraRotationSpeed * ts;

        BlueMarble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        BlueMarble::RenderCommand::Clear();

        oCamera.SetPosition(oCameraPosition);
        oCamera.SetRotation(oCameraRotation);

        BlueMarble::Renderer::BeginScene(oCamera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        for (int y = 0; y < 20; ++y) 
        {
            for (int x = 0; x < 20; ++x)
            {
                glm::vec3 pos(x * 0.1f, y * 0.1f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

                oFlatShader->UploadUniformFloat4("uColor", glm::vec4(1.0f, 1.0f - (y+x)/60.0f, 1.0f - (x+y)/40.0f, 0.0f));

                BlueMarble::Renderer::Submit(oFlatShader, oSquareVA, oTexture, transform);
            }
        }
        //BlueMarble::Renderer::Submit(oShader, oVertexArray, oTexture);

        BlueMarble::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {

    }

	void OnEvent(BlueMarble::Event& event) override
	{
		//BM_TRACE("{0}, event");
        /*BlueMarble::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<BlueMarble::KeyPressEvent>(BM_BIND_EVENT_FN(ExampleLayer::OnKeyPressEvent));*/
	}

private:
    std::shared_ptr<BlueMarble::Shader> oShader;
    std::shared_ptr<BlueMarble::VertexArray> oVertexArray;

    std::shared_ptr<BlueMarble::Shader> oFlatShader;
    std::shared_ptr<BlueMarble::VertexArray> oSquareVA;

    std::shared_ptr<BlueMarble::Texture> oTexture;

    BlueMarble::OrthographicCamera oCamera;
    glm::vec3 oCameraPosition;
    float oCameraRotation = 0.0f;
    float oCameraMoveSpeed = 2.0f;
    float oCameraRotationSpeed = 180.0f;

};

class Sandbox : public BlueMarble::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}  

	~Sandbox()
	{

	}

};

BlueMarble::Application* BlueMarble::CreateApplication()
{
	return new Sandbox();
}