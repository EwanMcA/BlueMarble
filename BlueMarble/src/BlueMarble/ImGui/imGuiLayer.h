#pragma once

#include "BlueMarble/layer.h"
#include "BlueMarble/Events/applicationEvent.h"
#include "BlueMarble/Events/keyEvent.h"
#include "BlueMarble/Events/mouseEvent.h"

namespace BlueMarble {

	class BLUEMARBLE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMousePressEvent(MousePressEvent& e);
		bool OnMouseReleaseEvent(MouseReleaseEvent& e);
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseScrollEvent(MouseScrollEvent& e);
		bool OnKeyPressEvent(KeyPressEvent& e);
		bool OnKeyReleaseEvent(KeyReleaseEvent& e);
		bool OnKeyTypeEvent(KeyTypeEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float oTime = 0.0f;
	};

}