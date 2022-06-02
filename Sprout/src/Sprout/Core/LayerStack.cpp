#include "spch.h"
#include "LayerStack.h"

namespace Sprout
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		Layers.emplace(Layers.begin() + StackPtr, layer);
		StackPtr++;
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(Layers.begin(), Layers.begin() + StackPtr, layer);
		if (it != Layers.begin() + StackPtr)
		{
			layer->OnDetach();
			Layers.erase(it);
			StackPtr--;
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(Layers.begin() + StackPtr, Layers.end(), layer);
		if (it != Layers.begin() + StackPtr)
		{
			layer->OnDetach();
			Layers.erase(it);
		}
	}
}
