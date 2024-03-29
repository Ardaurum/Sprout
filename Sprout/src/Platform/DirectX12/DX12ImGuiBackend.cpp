#include "spch.h"
#include "DX12ImGuiBackend.h"
#include "Sprout/Core/App.h"

#include "DX12Context.h"

#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_sdl3.h>

namespace Sprout
{
	void DX12ImGuiBackend::Init(const Window& window)
	{
		SDL_Window* nativeWindow = static_cast<SDL_Window*>(window.GetNativeWindow());
		DX12Context* dx12Context = static_cast<DX12Context*>(window.GetRendererContext());

		ImGui_ImplSDL3_InitForD3D(nativeWindow);
		// TODO IMPORTANT:
		//ImGui_ImplDX12_Init(dx12Context->GetDevice(), dx12Context->FRAME_COUNT);
	}

	void DX12ImGuiBackend::Shutdown()
	{
		ImGui_ImplDX12_Shutdown();
	}

	void DX12ImGuiBackend::NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
	}

	void DX12ImGuiBackend::RenderDrawData()
	{
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData());
	}
}
