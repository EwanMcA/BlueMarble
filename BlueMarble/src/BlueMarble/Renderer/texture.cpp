#include "bmpch.h"
#include "texture.h"

#include "renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace BlueMarble {

    Texture* Texture::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLTexture(path);
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace BlueMarble