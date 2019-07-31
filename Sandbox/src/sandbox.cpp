
#include <BlueMarble.h>
#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public BlueMarble::Layer
{
public:
	ExampleLayer()
        : Layer("Example"), 
          oCamera(0.77f, 
                  (float)BlueMarble::Application::Get().GetWindow().GetWidth() / 
                  (float)BlueMarble::Application::Get().GetWindow().GetHeight(), 
                   0.1f, 
                   10.0f), 
        oCameraPosition({ 0.0f, 0.0f, 1.0f })
	{
        oSquareVA.reset(BlueMarble::VertexArray::Create());

        std::vector<float> squareVertices;
        for (int i = -20; i < 21; ++i)
        {
            for (int j = -20; j < 21; ++j)
            {
                //-0.5f, -0.5f, 0.0f,     0.5f, 0.5f,
                // 0.5f, -0.5f, 0.0f,     1.0f, 0.5f,
                // 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
                //-0.5f,  0.5f, 0.0f,     0.5f, 1.0f
                squareVertices.insert(squareVertices.end(), { i * 0.5f - 0.5f, j * 0.5f - 0.5f, 0.0f,     0.5f, 0.5f });
                squareVertices.insert(squareVertices.end(), { i * 0.5f + 0.5f, j * 0.5f - 0.5f, 0.0f,     1.0f, 0.5f });
                squareVertices.insert(squareVertices.end(), { i * 0.5f + 0.5f, j * 0.5f + 0.5f, 0.0f,     1.0f, 1.0f });
                squareVertices.insert(squareVertices.end(), { i * 0.5f - 0.5f, j * 0.5f + 0.5f, 0.0f,     0.5f, 1.0f });
            }
        }

        std::shared_ptr<BlueMarble::VertexBuffer> squareVB;
        squareVB.reset(BlueMarble::VertexBuffer::Create(squareVertices.data(), squareVertices.size() * sizeof(float)));
        squareVB->SetLayout({ { BlueMarble::ShaderDataType::Float3, "aPosition" },
                              { BlueMarble::ShaderDataType::Float2, "aTexCoord" } });
        oSquareVA->AddVertexBuffer(squareVB);

        std::vector<uint32_t> squareIndices;
        for (uint32_t i = 0; i < 1681; ++i)
        {
            // TODO: Not achieving maximum reuse here. After the first square, can reuse 3 of its vertices in the 2nd square.
            //= { 0, 1, 2, 2, 3, 0 };
            squareIndices.insert(squareIndices.end(), { 0 + i * 4, 1 + i * 4, 2 + i * 4, 2 + i * 4, 3 + i * 4, 0 + i * 4 });
        }
        std::shared_ptr<BlueMarble::IndexBuffer> squareIB;
        squareIB.reset(BlueMarble::IndexBuffer::Create(squareIndices.data(), squareIndices.size()));
        oSquareVA->SetIndexBuffer(squareIB);

        oFlatShader.reset(new BlueMarble::Shader("shaders/shader.glsl"));

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

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_W)) {
            if (oCameraPosition.z > 0.5f) {
                oCameraPosition.z -= oCameraMoveSpeed * ts;
                if (oCameraPosition.z < 1.5f) {
                    oCameraRotation.x += oCameraRotationSpeed * ts;
                    oCameraRotation.x = std::min(80.0f, oCameraRotation.x);
                }
            }
        }
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_S)) {
            if (oCameraPosition.z < 4.5f) {
                oCameraPosition.z += oCameraMoveSpeed * ts;
                oCameraRotation.x -= oCameraRotationSpeed * ts;
                oCameraRotation.x = std::max(0.0f, oCameraRotation.x);
            }
        }

        if (BlueMarble::Input::IsKeyPressed(BM_KEY_A))
            oCameraRotation.z += oCameraRotationSpeed * ts;
        else if (BlueMarble::Input::IsKeyPressed(BM_KEY_D))
            oCameraRotation.z -= oCameraRotationSpeed * ts;

        BlueMarble::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        BlueMarble::RenderCommand::Clear();

        oCamera.SetPosition(oCameraPosition);
        oCamera.SetRotation(oCameraRotation);

        BlueMarble::Renderer::BeginScene(oCamera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        //glm::vec3 pos(x * 0.1f, y * 0.1f, 0.0f);
        glm::mat4 transform = glm::mat4(1.0f) * scale;
        //oFlatShader->UploadUniformFloat4("uColor", glm::vec4(1.0f, 1.0f - (y+x)/60.0f, 1.0f - (x+y)/40.0f, 0.0f));

        BlueMarble::Renderer::Submit(oFlatShader, oSquareVA, oTexture, transform);

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
    std::shared_ptr<BlueMarble::Shader> oFlatShader;
    std::shared_ptr<BlueMarble::VertexArray> oSquareVA;

    std::shared_ptr<BlueMarble::Texture> oTexture;

    BlueMarble::PerspectiveCamera oCamera;
    glm::vec3 oCameraPosition;
    glm::vec3 oCameraRotation = glm::vec3(0.0f);
    float oCameraMoveSpeed = 2.0f;
    float oCameraRotationSpeed = 90.0f;

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