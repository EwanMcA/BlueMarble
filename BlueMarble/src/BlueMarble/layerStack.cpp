#include "bmpch.h"
#include "layerStack.h"

namespace BlueMarble {

	LayerStack::LayerStack()
	{
	}

	// Layers are deallocated if the stack is destructed,
	// but not otherwise
	LayerStack::~LayerStack()
	{
		for (Layer* layer : oLayers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		oLayers.emplace(oLayers.begin() + oLayerInsertIndex, layer);
		oLayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		oLayers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(oLayers.begin(), oLayers.end(), layer);
		if (it != oLayers.end())
		{
			oLayers.erase(it);
			oLayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(oLayers.begin(), oLayers.end(), overlay);
		if (it != oLayers.end())
			oLayers.erase(it);
	}

}