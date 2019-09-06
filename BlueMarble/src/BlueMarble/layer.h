#pragma once

#include "BlueMarble/core.h"
#include "BlueMarble/Events/event.h"
#include "BlueMarble/Core/timeStep.h"

namespace BlueMarble {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return oDebugName; }
	protected:
		std::string oDebugName;

	};

}