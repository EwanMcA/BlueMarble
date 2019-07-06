#include "bmpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace BlueMarble {

    // TODO: When there are multiple graphics api's this will change
    RendererAPI* RenderCommand::cRendererAPI = new OpenGLRendererAPI;

} // namespace BlueMarble