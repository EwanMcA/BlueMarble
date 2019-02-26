#pragma once

#include "BlueMarble/core.h"
#include "BlueMarble/Events/event.h"

namespace BlueMarble {

	class BLUEMARBLE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return oDebugName; }
	protected:
		std::string oDebugName;
	};

}