#include "bmpch.h"
#include "renderer.h"

namespace BlueMarble {

    void Renderer::BeginScene()
    {
        // Get the data to put in uniforms to the shaders
        // cubemaps, projection matrix, view matrix, lighting stuff
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }


} // namespace BlueMarble