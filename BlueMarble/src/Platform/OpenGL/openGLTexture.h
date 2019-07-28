#pragma once

#include "BlueMarble/core.h"
#include "BlueMarble/Renderer/texture.h"

namespace BlueMarble {

	class OpenGLTexture : public Texture
	{
	public:
        OpenGLTexture(const std::string& path);
		~OpenGLTexture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return oWidth; }
		inline int GetHeight() const { return oHeight; }
	private:
		unsigned int oRendererID;
		std::string oFilePath;
		unsigned char* oLocalBuffer;
		int oWidth, oHeight, oBPP;
	};

} // Namespace BlueMarble
