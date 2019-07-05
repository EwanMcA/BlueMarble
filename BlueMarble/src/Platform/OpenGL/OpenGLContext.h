#pragma once

#include "Bluemarble/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace BlueMarble {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* oWindowHandle;
    };

} // namespace BlueMarble
