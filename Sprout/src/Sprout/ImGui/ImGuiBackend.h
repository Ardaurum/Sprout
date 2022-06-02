#pragma once

#include "Sprout/Core/Window.h"
#include "Sprout/Renderer/RendererAPI.h"

namespace Sprout
{
	class ImGuiBackend
	{
	public:
		virtual void Init(const Window& window) = 0;
		virtual void Shutdown() = 0;
		virtual void NewFrame() = 0;
		virtual void RenderDrawData() = 0;

		static std::shared_ptr<ImGuiBackend> Create(const RendererAPI::API api);
	};
}