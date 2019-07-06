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

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return oVertexBuffers; }
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return oIndexBuffer; }
    private:
        std::vector<std::shared_ptr<VertexBuffer>> oVertexBuffers;
        std::shared_ptr<IndexBuffer> oIndexBuffer;

        uint32_t oRendererID;
    };

} // namespace BlueMarble
