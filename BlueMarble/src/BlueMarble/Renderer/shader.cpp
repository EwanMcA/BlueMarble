#include "bmpch.h"
#include "Shader.h"

#include "renderer.h"
#include "Platform/OpenGL/openGLShader.h"

namespace BlueMarble {

    Shader* Shader::Create(const std::string& filename)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLShader(filename);
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace BlueMarble