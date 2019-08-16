#pragma once

#include "BlueMarble/core.h"

#include "BlueMarble/Renderer/camera.h"

namespace BlueMarble {
	
	class Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return cInstance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return cInstance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return cInstance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return cInstance->GetMouseXImpl(); }
		inline static float GetMouseY() { return cInstance->GetMouseYImpl(); }
        inline static float GetMouseZ() { return cInstance->GetMouseZImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
        virtual float GetMouseZImpl() = 0;
	private:
		static Input* cInstance;

	};
}