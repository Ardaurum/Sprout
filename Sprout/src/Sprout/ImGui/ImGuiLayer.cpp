#include "spch.h"
#include "ImGuiLayer.h"
#include "Sprout/Core/App.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

namespace Sprout
{
	ImGuiLayer::ImGuiLayer(RendererAPI::API api)
		: Layer("ImGuiLayer"),
		  Backend(ImGuiBackend::Create(api))
	{ }

	ImGuiLayer::~ImGuiLayer()
	{ }

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		App& app = App::Get();
		Backend->Init(app.GetWindow());
	}

	void ImGuiLayer::OnDetach()
	{
		Backend->Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		Backend->NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		App& app = App::Get();
		io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		Backend->RenderDrawData();

		if (io.ConfigFlags && ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::OnEvent(Event& ev)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ev.IsInCategory(EventCategory::Mouse) && io.WantCaptureMouse) ev.SetHandled();
		if (ev.IsInCategory(EventCategory::Keyboard) && io.WantCaptureKeyboard) ev.SetHandled();
	}
}