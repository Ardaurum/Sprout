#pragma once

#include "Core.h"
#include "Window.h"

#include "Sprout/CoreEvents/CoreEvent.h"
#include "Sprout/CoreEvents/WinEvent.h"

#include "Sprout/Core/LayerStack.h"
#include "Sprout/ImGui/ImGuiLayer.h"

#include "Sprout/Renderer/Buffer.h"
#include "Sprout/Renderer/VertexArray.h"
#include "Sprout/Renderer/Camera.h"

namespace Sprout
{
	class App
	{
	public:
		App(std::string const& name);
		virtual ~App();

		void Run();
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		static inline App& Get() { return *Instance; }

		Window& GetWindow() { return *AppWindow; }
		ImGuiLayer* GetImGuiLayer() { return GuiLayer; }

	private:
		void OnEvent(Event& ev);

		bool OnWindowClose(WinCloseEvent& ev);
		bool OnWindowResize(WinResizeEvent& ev);
		bool OnWindowMinimized(WinMinimizeEvent& ev);
		bool OnWindowRestored(WinRestoreEvent& ev);

		bool Running = true;
		bool WinMinimized = false;
		std::shared_ptr<Window> AppWindow;

		std::chrono::high_resolution_clock::time_point LastFrameTime;
		
		LayerStack Layers;
		ImGuiLayer* GuiLayer;

		static App* Instance;
	};

	void DefineCLIArgs();

	void DefineGameCLIArgs();
	App* CreateApp();
}
