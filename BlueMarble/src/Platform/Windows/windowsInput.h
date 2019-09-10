#pragma once

#include "BlueMarble/application.h"
#include "BlueMarble/input.h"

namespace BlueMarble {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual float GetMouseZImpl() override;
        virtual std::pair<float, float> GetNormMousePositionImpl() override;
        virtual float GetNormMouseXImpl() override 
            { return GetMouseXImpl() / (Application::Get().GetWindow().GetWidth() * 0.5f) - 1.0f; }
        virtual float GetNormMouseYImpl() override
            { return GetMouseYImpl() / (Application::Get().GetWindow().GetHeight() * 0.5f) - 1.0f; }
        virtual float GetNormMouseZImpl() override
            { return 2 * GetMouseZImpl() - 1.0f; }
	};

}
 