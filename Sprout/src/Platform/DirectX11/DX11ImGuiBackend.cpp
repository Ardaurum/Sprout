#include "spch.h"
#include "DX11ImGuiBackend.h"
#include "Sprout/Core/App.h"

#include "DX11Context.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_sdl3.h>

namespace Sprout
{
	void DX11ImGuiBackend::Init(const Window& window)
	{
		SDL_Window* nativeWindow = static_cast<SDL_Window*>(window.GetNativeWindow());
		DX11Context* dx11Context = static_cast<DX11Context*>(window.GetRendererContext());

		ImGui_ImplSDL3_InitForD3D(nativeWindow);
		ImGui_ImplDX11_Init(dx11Context->GetDevice(), dx11Context->GetDeviceContext());
	}

	void DX11ImGuiBackend::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void DX11ImGuiBackend::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}

	void DX11ImGuiBackend::RenderDrawData()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
