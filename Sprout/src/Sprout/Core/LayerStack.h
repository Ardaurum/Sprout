#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Sprout
{
	class LayerStack
	{
	public:
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		std::vector<Layer*>::iterator end() { return Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return Layers.rend(); }

	private:
		std::vector<Layer*> Layers;
		uint16_t StackPtr = 0;
	};
}