#include "bmpch.h"
#include "renderer.h"

namespace BlueMarble {

    Renderer::SceneData* Renderer::cSceneData = new Renderer::SceneData;

    void Renderer::BeginScene(Camera& camera)
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
                          const std::vector<std::shared_ptr<Texture>>& textures,
                          const glm::mat4& transform)
    {
        shader->Bind();
        // We don't need to do this on every call... should batch it
        shader->UploadUniformMat4("uViewProjection", cSceneData->ProjectionViewMatrix);
        // This is specific to the object, so it does need to be called every time
        shader->UploadUniformMat4("uTransform", transform);
        shader->UploadUniformInt("uTexture0", 0);
        shader->UploadUniformInt("uTexture1", 1);
        shader->UploadUniformInt("uTexture2", 2);

        vertexArray->Bind();
        for (int i = 0; i < textures.size(); ++i) {
            textures[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(vertexArray);
    }


} // namespace BlueMarble