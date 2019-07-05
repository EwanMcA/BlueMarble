#pragma once

#include "BlueMarble/layer.h"
#include "BlueMarble/Events/applicationEvent.h"
#include "BlueMarble/Events/keyEvent.h"
#include "BlueMarble/Events/mouseEvent.h"

namespace BlueMarble {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float oTime = 0.0f;
	};

}