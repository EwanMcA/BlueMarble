#include "bmpch.h"

#include "OpenGLTexture.h"
#include "glError.h"

#include "stb_image.h"

namespace BlueMarble {

    OpenGLTexture::OpenGLTexture(const std::string& path)
        : oFilePath(path), oLocalBuffer(nullptr), oWidth(0), oHeight(0), oBPP(0)
    {
        stbi_set_flip_vertically_on_load(1);
        oLocalBuffer = stbi_load(path.c_str(), &oWidth, &oHeight, &oBPP, 4);

        GL_CALL(glGenTextures(1, &oRendererID));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, oRendererID));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, oWidth, oHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, oLocalBuffer));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        if (oLocalBuffer) {
            stbi_image_free(oLocalBuffer);
        }
    }

    OpenGLTexture::~OpenGLTexture()
    {
        GL_CALL(glDeleteTextures(1, &oRendererID));
    }

    void OpenGLTexture::Bind(unsigned int slot) const
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + slot)); // Can do this because they're consecutive ints
        GL_CALL(glBindTexture(GL_TEXTURE_2D, oRendererID));
    }

    void OpenGLTexture::Unbind() const
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

} // Namespace BlueMarble