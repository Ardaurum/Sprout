#pragma once

#include "Sprout/CoreEvents/CoreEvent.h"
#include "Timestep.h"

namespace Sprout
{
	class Layer
	{
	public:
		Layer(const std::string& name) : Name(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void Update(Timestep ts) {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& ev) {};

		inline std::string_view GetName() const { return Name; }
	private:
		std::string Name;
	};
}