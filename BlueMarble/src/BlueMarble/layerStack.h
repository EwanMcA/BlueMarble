#pragma once

#include "BlueMarble/core.h"
#include "layer.h"

#include <vector>

namespace BlueMarble {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return oLayers.begin(); }
		std::vector<Layer*>::iterator end() { return oLayers.end(); }
	private:
		std::vector<Layer*> oLayers;
		unsigned int oLayerInsertIndex = 0;
	};

} // namespace BlueMarble
