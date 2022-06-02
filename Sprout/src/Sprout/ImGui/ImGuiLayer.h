#pragma once

#include "ImGuiBackend.h"

#include "Sprout/Core/Layer.h"
#include "Sprout/Renderer/RendererAPI.h"

namespace Sprout
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(RendererAPI::API api);
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void Begin();
		void End();
		void OnEvent(Event& ev) override;

	private:
		std::shared_ptr<ImGuiBackend> Backend;
	};
}