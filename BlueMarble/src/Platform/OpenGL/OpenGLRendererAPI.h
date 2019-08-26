#pragma once

#include "BlueMarble/Renderer/RendererAPI.h"

namespace BlueMarble {

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;   

        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };

} // namespace BlueMarble
