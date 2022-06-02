#include "spch.h"
#include "DX11ImGuiBackend.h"
#include "Sprout/Core/App.h"

#include "DX11Context.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_glfw.h>

namespace Sprout
{
	void DX11ImGuiBackend::Init(const Window& window)
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		DX11Context* dx11Context = static_cast<DX11Context*>(window.GetRendererContext());

		ImGui_ImplGlfw_InitForOther(nativeWindow, true);
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
