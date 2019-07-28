#include "bmpch.h"
#include "renderer.h"

namespace BlueMarble {

    Renderer::SceneData* Renderer::cSceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        // Get the data to put in uniforms to the shaders
        // cubemaps, projection matrix, view matrix, lighting stuff

        cSceneData->ProjectionViewMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, 
                          const std::shared_ptr<VertexArray>& vertexArray,
                          const std::shared_ptr<Texture>& texture,
                          const glm::mat4& transform)
    {
        shader->Bind();
        // We don't need to do this on every call... should batch it
        shader->UploadUniformMat4("uViewProjection", cSceneData->ProjectionViewMatrix);
        // This is specific to the object, so it does need to be called every time
        shader->UploadUniformMat4("uTransform", transform);

        vertexArray->Bind();
        texture->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }


} // namespace BlueMarble