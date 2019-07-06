#pragma once

#include "RendererAPI.h"

namespace BlueMarble {

    class RenderCommand
    {
    public:
        inline static void SetClearColor(const glm::vec4& color)
        {
            cRendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            cRendererAPI->Clear();
        }

        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
        {
            cRendererAPI->DrawIndexed(vertexArray);
        }
    private:
        static RendererAPI* cRendererAPI;
    };

} // namespace BlueMarble
