#include "bmpch.h"
#include "material.h"

#include "renderer.h"
#include "Platform/OpenGL/openGLMaterial.h"

namespace BlueMarble {

    Material* Material::Create(Ref<Shader> shader)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLMaterial(std::dynamic_pointer_cast<OpenGLShader>(shader));
        }

        BM_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}