#pragma once

#include "Sprout/Core/Window.h"

struct GLFWwindow;

namespace Sprout
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProperties& properties);
		virtual ~WinWindow();

		virtual void Update() override;

		uint32_t GetWidth() const override { return WinData.Width; }
		uint32_t GetHeight() const override { return WinData.Height; }
		float GetAspect() const override { return (float) WinData.Width / WinData.Height; }

		void SetCallbackFn(const EventCallbackFn& callback) override { WinData.EventCallback = callback; }

		virtual void* GetNativeWindow() const { return Window; }
		virtual void* GetRendererContext() const { return Context.get(); }

	private:
		void SetupWindowEvents();
		void SetupMouseEvents();
		void SetupKeyEvents();
		void Shutdown();

		GLFWwindow* Window;
		std::shared_ptr<class RendererContext> Context;

		struct Data
		{
			std::string Title;
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool IsMinimized = false;

			EventCallbackFn EventCallback;
		} WinData;
	};
}
