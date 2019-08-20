#pragma once

#include "renderCommand.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"

namespace BlueMarble {
    
	class Renderer
	{
	public:
        //TODO take in the rest of the scene parameters
        static void BeginScene(Camera& camera); 
        static void EndScene();
        
        static void Submit(const Ref<Shader>& shader,
                           const Ref<VertexArray>& vertexArray,
                           const std::vector<Ref<Texture2D>>& textures,
                           const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ProjectionViewMatrix;
        };

        static SceneData* cSceneData;
	}; 

} // Namespace BlueMarble