#pragma once

#include "spch.h"
#include "App.h"

#include "Layer.h"
#include "LayerStack.h"
#include "Timestep.h"
#include "Sprout/Renderer/Renderer.h"

#include <imgui.h>

namespace Sprout {
	App* App::Instance = nullptr;

	App::App() 
	{
		SPROUT_CORE_ASSERT(!Instance, "App already exists!");
		Instance = this;

		Renderer::PrepareAPI(RendererAPI::API::OpenGL);

		AppWindow = Window::Create();
		AppWindow->SetCallbackFn([this](Event& ev) { OnEvent(ev); });

		Renderer::Init();

		GuiLayer = new ImGuiLayer(Renderer::GetAPI());
		PushOverlay(GuiLayer);
	}

	App::~App() 
	{
		
	}

	void App::Run() 
	{
		while(Running)
		{
			auto time = std::chrono::high_resolution_clock::now();
			Timestep timestep = std::chrono::duration<float>(time - LastFrameTime).count();
			LastFrameTime = time;

			SPROUT_CORE_INFO("Time: {0}, FPS: {1}", float(timestep), floor(1.0f / float(timestep)));

			if (WinMinimized == false)
			{ 
				for (Layer* layer : Layers)
				{
					layer->Update(timestep);
				}

				GuiLayer->Begin();
				for (Layer* layer : Layers)
				{
					layer->OnImGuiRender();
				}
				GuiLayer->End();
			}

			AppWindow->Update();
		}
	}

	void App::OnEvent(Event& ev)
	{
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WinCloseEvent>([this](WinCloseEvent& ev) { return OnWindowClose(ev); });
		dispatcher.Dispatch<WinResizeEvent>([this](WinResizeEvent& ev) { return OnWindowResize(ev); });
		dispatcher.Dispatch<WinMinimizeEvent>([this](WinMinimizeEvent& ev) { return OnWindowMinimized(ev); });
		dispatcher.Dispatch<WinRestoreEvent>([this](WinRestoreEvent& ev) { return OnWindowRestored(ev); });

		for (auto it = Layers.rbegin(); it != Layers.rend(); ++it)
		{
			if (ev.IsHandled())
				break;

			(*it)->OnEvent(ev);
		}
	}

	bool App::OnWindowClose(WinCloseEvent& ev)
	{
		Close();
		return true;
	}

	bool App::OnWindowResize(WinResizeEvent& ev)
	{
		return false;
	}

	bool App::OnWindowMinimized(WinMinimizeEvent& ev)
	{
		WinMinimized = true;
		return true;
	}

	bool App::OnWindowRestored(WinRestoreEvent& ev)
	{
		WinMinimized = false;
		return true;
	}

	void App::Close()
	{
		Running = false;
	}

	void App::PushLayer(Layer* layer)
	{
		Layers.PushLayer(layer);
		layer->OnAttach();
	}

	void App::PushOverlay(Layer* layer)
	{
		Layers.PushOverlay(layer);
		layer->OnAttach();
	}
}
