#include "bmpch.h"
#include "Buffer.h"

#include "renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace BlueMarble {

    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace BlueMarble