#pragma once

#include "BlueMarble/Renderer/RendererAPI.h"

namespace BlueMarble {

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };

} // namespace BlueMarble
