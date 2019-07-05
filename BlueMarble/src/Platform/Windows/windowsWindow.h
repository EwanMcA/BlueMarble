#pragma once

#include "BlueMarble/Window.h"
#include "BlueMarble/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace BlueMarble {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return oData.Width; }
		inline unsigned int GetHeight() const override { return oData.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { oData.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return oWindow; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* oWindow;
        GraphicsContext* oContext;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData oData;
	};

}