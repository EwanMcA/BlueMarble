#pragma once

#include "RenderCommand.h"

namespace BlueMarble {
    
	class Renderer
	{
	public:
        static void BeginScene(); //TODO take in scene parameters
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

} // Namespace BlueMarble