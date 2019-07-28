#pragma once

#include <glad/glad.h>
#include <BlueMarble/log.h>

#define GL_CALL(fn) do { BlueMarble::GLClearError(); \
						 fn;\
						 BM_CORE_ASSERT(BlueMarble::GLLogCall(#fn, __FILE__, __LINE__), "OpenGL Error: ");\
					   } while (0)

namespace BlueMarble {

	inline static void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	inline static bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			BM_CORE_ERROR("({0}): {1} {2}: {3}", error, function, file, line);
			return false;
		}
		return true;
	}
}
