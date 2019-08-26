#include "bmpch.h"
#include "openGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace BlueMarble {

    OpenGLTexture2D::OpenGLTexture2D(const std::string & path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        BM_CORE_ASSERT(data, "Failed to load image!");
        
        oWidth = width;
        oHeight = height;

        GLenum internalFormat{ 0 };
        GLenum dataFormat{ 0 };
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        BM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glGenTextures(1, &oRendererID);
        glBindTexture(GL_TEXTURE_2D, oRendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, oWidth, oHeight, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (data) {
            stbi_image_free(data);
        }
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &oRendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot); // Can do this because they're consecutive ints
        glBindTexture(GL_TEXTURE_2D, oRendererID);
    }

}

