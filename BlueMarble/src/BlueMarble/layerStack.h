#pragma once

#include "BlueMarble/core.h"
#include "layer.h"

#include <vector>

namespace BlueMarble {

	class BLUEMARBLE_API LayerStack
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
		std::vector<Layer*>::iterator oLayerInsert;
	};

} // namespace BlueMarble
