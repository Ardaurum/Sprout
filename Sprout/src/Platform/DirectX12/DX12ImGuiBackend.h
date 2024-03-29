#pragma once

#include "Sprout/ImGui/ImGuiBackend.h"

namespace Sprout
{
	class DX12ImGuiBackend : public ImGuiBackend
	{
	public:
		virtual void Init(const Window& window) override;
		virtual void Shutdown() override;
		virtual void NewFrame() override;
		virtual void RenderDrawData() override;
	};
}