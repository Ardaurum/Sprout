#include "spch.h"
#include "App.h"

#include "CLISystem.h"
#include "Layer.h"
#include "Timestep.h"
#include "Sprout/Renderer/Renderer.h"

namespace Sprout 
{
	void DefineCLIArgs()
	{
		SCLI_DEFINE_ARG("width", CLISystem::Type::Number, 'w', (void*) 1920, "Window width");
		SCLI_DEFINE_ARG("height", CLISystem::Type::Number, 'h', (void*) 1080, "Window height");
		SCLI_DEFINE_ARG("use_warp", CLISystem::Type::Boolean, 0, (void*) false, "Use Windows Advanced Rasterization Platform");
	}

	App* App::Instance = nullptr;

	App::App(std::string const& name) 
	{
		SPROUT_CORE_ASSERT_MSG(!Instance, "App already exists!");
		Instance = this;

		Renderer::PrepareAPI(RendererAPI::API::Direct3D12);

		CLISystem const& cliSystem = CLISystem::Get();
		uint32_t winWidth = 1920;
		uint32_t winHeight = 1080;
		if (cliSystem.HasValue(SCLI_ID(width))) winWidth = static_cast<uint32_t>(cliSystem.GetNumber(SCLI_ID(width)));
		if (cliSystem.HasValue(SCLI_ID(height))) winHeight = static_cast<uint32_t>(cliSystem.GetNumber(SCLI_ID(height)));

		AppWindow = Window::Create(WindowProperties(name, winWidth, winHeight));
		AppWindow->InitContext();
		AppWindow->SetCallbackFn([this](Event& ev) { OnEvent(ev); });

		Renderer::Init();

		GuiLayer = new ImGuiLayer(Renderer::GetAPI());
		PushOverlay(GuiLayer);
	}

	App::~App() 
	{
		Renderer::Destroy();
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
