#include "bmpch.h"
#include "texture.h"

#include "renderer.h"
#include "Platform/OpenGL/openGLTexture.h"

namespace BlueMarble {

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace BlueMarble