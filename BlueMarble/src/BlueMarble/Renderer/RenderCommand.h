#pragma once

#include "RendererAPI.h"

namespace BlueMarble {

    class RenderCommand
    {
    public:

        inline static void Init()
        {
            cRendererAPI->Init();
        }


        inline static void SetClearColor(const glm::vec4& color)
        {
            cRendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            cRendererAPI->Clear();
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            cRendererAPI->DrawIndexed(vertexArray);
        }
    private:
        static RendererAPI* cRendererAPI;
    };

} // namespace BlueMarble
