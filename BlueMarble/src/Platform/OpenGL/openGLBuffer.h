#pragma once

#include "BlueMarble/Renderer/buffer.h"

namespace BlueMarble {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const BufferLayout GetLayout() const override { return oLayout; };
        virtual void SetLayout(const BufferLayout& layout) override { oLayout = layout; }
    private:
        uint32_t oRendererID;
        BufferLayout oLayout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;

        virtual uint32_t GetCount() const { return oCount; }
    private:
        uint32_t oRendererID;
        uint32_t oCount;
    };

} // namespace BlueMarble
