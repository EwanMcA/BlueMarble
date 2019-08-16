#include "bmpch.h"
#include "windowsInput.h"

#include "BlueMarble/application.h"

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

    float WindowsInput::GetMouseZImpl()
    {
        unsigned int width = Application::Get().GetWindow().GetWidth();
        unsigned int height = Application::Get().GetWindow().GetHeight();
        //glfwGetFramebufferSize(win, &pixel_w, &pixel_h);
        auto p = GetMousePositionImpl();
        glm::vec2 screenPos{ p.first, p.second };
        //glm::vec2 pixel_pos = screen_pos * glm::vec2(pixel_w, pixel_h) / glm::vec2(screen_w, screen_h); // note: not necessarily integer
        //pixel_pos = pixel_pos + glm::vec2(0.5f, 0.5f); // shift to GL's center convention
        glm::vec3 win = glm::vec3(screenPos.x, screenPos.y - 1 - screenPos.y, 0.0f);
        glReadPixels((GLint)win.x, (GLint)win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);

        return win.z;
    }

}