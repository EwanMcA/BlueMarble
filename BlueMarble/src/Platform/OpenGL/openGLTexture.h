#pragma once

#include "BlueMarble/Renderer/texture.h"

namespace BlueMarble {
    
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        virtual uint32_t GetWidth() const override { return oWidth; }
        virtual uint32_t GetHeight() const override { return oHeight; }

        virtual void Bind(uint32_t slot = 0) const override;
    private:
        std::string oPath;
        
        uint32_t oWidth, oHeight;
        uint32_t oRendererID;

    };
}
