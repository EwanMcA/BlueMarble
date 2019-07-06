#include "bmpch.h"
#include "vertexArray.h"
#include "renderer.h"

#include "Platform/OpenGL/openGLVertexArray.h"

namespace BlueMarble {

    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace BlueMarble