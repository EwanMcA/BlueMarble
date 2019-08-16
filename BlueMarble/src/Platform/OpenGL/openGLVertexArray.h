#pragma once

#include "BlueMarble/Renderer/vertexArray.h"

namespace BlueMarble {

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return oVertexBuffers; }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return oIndexBuffer; }
    private:
        std::vector<Ref<VertexBuffer>> oVertexBuffers;
        Ref<IndexBuffer> oIndexBuffer;

        uint32_t oRendererID;
    };

} // namespace BlueMarble
