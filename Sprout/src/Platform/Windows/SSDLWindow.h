#pragma once

#include "Sprout/Core/Window.h"
#include <SDL3/SDL.h>

namespace Sprout
{
	class SSDLWindow : public Window
	{
	public:
		SSDLWindow(const WindowProperties& properties);
		virtual ~SSDLWindow();

		virtual void InitContext() override;
		virtual void Update() override;

		uint32_t GetWidth() const override { return WinData.Width; }
		uint32_t GetHeight() const override { return WinData.Height; }
		float GetAspect() const override { return (float) WinData.Width / WinData.Height; }

		void SetCallbackFn(const EventCallbackFn& callback) override { WinData.EventCallback = callback; }

		virtual void* GetNativeWindowHandle() const { return SDL_GetProperty(SDL_GetWindowProperties(Window), "SDL.window.win32.hwnd", nullptr); }
		virtual void* GetNativeWindow() const { return Window; }
		virtual void* GetRendererContext() const { return Context.get(); }

	private:
		void Shutdown();
		
		void HandleSDLEvent(SDL_Event const& sdlEvent);
		void HandleSDLWindowEvent(SDL_WindowEvent const& sdlEvent);

		SDL_Window* Window;
		std::shared_ptr<class RendererContext> Context;

		struct Data
		{
			std::string Title;
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool IsMinimized = false;

			EventCallbackFn EventCallback;
		} WinData;

#ifdef SPROUT_DEBUG
		bool ContextInitialized;
#endif
	};
}
