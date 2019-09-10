#include "bmpch.h"
#include "WindowsWindow.h"

#include "BlueMarble/Events/ApplicationEvent.h"
#include "BlueMarble/Events/MouseEvent.h"
#include "BlueMarble/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

// TODO: Remove this
#include <glad/glad.h>

namespace BlueMarble {

	static bool cGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		BM_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		oData.Title = props.Title;
		oData.Width = props.Width;
		oData.Height = props.Height;

		BM_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!cGLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			BM_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			cGLFWInitialized = true;
		}

		oWindow = glfwCreateWindow((int)props.Width, (int)props.Height, oData.Title.c_str(), nullptr, nullptr);
        
        oContext = new OpenGLContext(oWindow);
        oContext->Init();
        //^
		glfwSetWindowUserPointer(oWindow, &oData);
		SetVSync(true);
		
        // Set GLFW callbacks
        glfwSetWindowSizeCallback(oWindow, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            // TODO: This shouldn't be here
            glViewport(0, 0, width, height);

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

		glfwSetWindowCloseCallback(oWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(oWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(oWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypeEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(oWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MousePressEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseReleaseEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(oWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(oWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMoveEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(oWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
        oContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		oData.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return oData.VSync;
	}

}