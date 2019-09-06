#include "bmpch.h"
#include "renderer.h"

#include "Platform/OpenGL/openGLShader.h"

namespace BlueMarble {

    Renderer::SceneData* Renderer::cSceneData = new Renderer::SceneData;

    
    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(Camera& camera)
    {
        // Get the data to put in uniforms to the shaders
        // cubemaps, projection matrix, view matrix, lighting stuff

        cSceneData->ProjectionViewMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<Material>& material,
                          const Ref<VertexArray>& vertexArray,
                          const glm::mat4& transform)
    {
        // TODO: Get rid of these dynamic casts when API agnostic material system is properly implemented
        // TODO: We don't need to do this on every call... should batch it
        material->SetMat4("uViewProjection", cSceneData->ProjectionViewMatrix);
        // This is specific to the object, so it does need to be called every time
        material->SetMat4("uTransform", transform);
        material->UploadUniforms();
        material->UploadTextures();

        vertexArray->Bind();
        
        RenderCommand::DrawIndexed(vertexArray);
    }


} // namespace BlueMarble