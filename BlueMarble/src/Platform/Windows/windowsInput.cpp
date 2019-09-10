#include "bmpch.h"
#include "windowsInput.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace BlueMarble {

	Input* Input::cInstance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}

    // TODO: Implement this properly
    float WindowsInput::GetMouseZImpl()
    {
        unsigned int width = Application::Get().GetWindow().GetWidth();
        unsigned int height = Application::Get().GetWindow().GetHeight();
        auto p = GetMousePositionImpl();
        glm::vec2 screenPos{ p.first, p.second };
        glm::vec3 win = glm::vec3(screenPos.x, height - screenPos.y, 0.0f);

        glReadPixels((GLint)win.x, (GLint)win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);

        return win.z;
    }

    std::pair<float, float> WindowsInput::GetNormMousePositionImpl()
    {
        auto& window = Application::Get().GetWindow();
        double xpos, ypos;
        glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);
        xpos = xpos / (Application::Get().GetWindow().GetWidth() * 0.5f) - 1.0f;
        ypos = ypos / (Application::Get().GetWindow().GetHeight() * 0.5f) - 1.0f;

        return { (float)xpos, (float)ypos };
    }
}