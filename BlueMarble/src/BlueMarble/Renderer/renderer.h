#pragma once

#include "BlueMarble/Core.h"

namespace BlueMarble {

	class Renderer
	{
	public:
		virtual ~Renderer() {}

		virtual void Clear() const = 0;
	};

} // Namespace BlueMarble