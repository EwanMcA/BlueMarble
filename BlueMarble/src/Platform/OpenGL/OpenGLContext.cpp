#include "bmpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "Glad/glad.h"

namespace BlueMarble{

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : oWindowHandle(windowHandle)
    {
        BM_CORE_ASSERT(windowHandle, "Window Handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(oWindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BM_CORE_ASSERT(status, "Failed to initialize Glad!");

        BM_CORE_INFO("OpenGL Info:");
        BM_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        BM_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        BM_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        glEnable(GL_DEPTH_TEST);
    }
    
    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(oWindowHandle);
    }

} // namespace BlueMarble