#include "spch.h"
#include "SSDLWindow.h"

#include "Sprout/CoreEvents/WinEvent.h"
#include "Sprout/CoreEvents/KeyEvent.h"
#include "Sprout/CoreEvents/MouseEvent.h"

#include "Sprout/Renderer/RendererContext.h"
#include "Sprout/Renderer/RendererAPI.h"
#include <backends/imgui_impl_sdl3.h>

namespace Sprout
{
	std::shared_ptr<Window> Window::Create(const WindowProperties& Properties)
	{
		return std::make_shared<SSDLWindow>(Properties);
	}

	static uint8_t sdlWindowCount = 0;

	static void ErrorCallback(int error_code, const char* description)
	{
		SPROUT_CORE_ERROR("GLFW Error: {0}. Description: {1}", error_code, description);
	}

	SSDLWindow::SSDLWindow(const WindowProperties& properties)
	{
		SPROUT_CORE_INFO("Creating window \"{0}\": {1}, {2}", properties.Title, properties.Width, properties.Height);

#ifdef SPROUT_DEBUG
		ContextInitialized = false;
#endif
		WinData.Title = properties.Title + ": " + RendererAPI::ToString(RendererAPI::GetAPI());
		WinData.Height = properties.Height;
		WinData.Width = properties.Width;

		if (sdlWindowCount == 0)
		{
			int success = SDL_Init(SDL_INIT_VIDEO);
			SPROUT_CORE_ASSERT_MSG(success == 0, "Failed to initialize SDL!");
		}

		uint32_t sdlFlags = 0;
		if (RendererAPI::GetAPI() != RendererAPI::API::Vulkan)
		{
			sdlFlags |= SDL_WINDOW_VULKAN;
		}

		Window = SDL_CreateWindow(properties.Title.c_str(), properties.Width, properties.Height, sdlFlags);

		if (Window == nullptr)
		{
			printf("Could not create window %s\n", SDL_GetError());
			return;
		}
		sdlWindowCount++;
	}

	SSDLWindow::~SSDLWindow()
	{
		Shutdown();
	}

	void SSDLWindow::InitContext()
	{
#if SPROUT_DEBUG
		ContextInitialized = true;
#endif
		Context = RendererContext::Create(this);
		Context->Init();
	}

	void SSDLWindow::Update()
	{
#if SPROUT_DEBUG
		SPROUT_CORE_ASSERT_MSG(ContextInitialized, "Update executed before initializing the window context!");
#endif
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent))
		{
			uint32_t windowID = SDL_GetWindowID(Window);
			if (sdlEvent.window.windowID == windowID)
			{
				HandleSDLEvent(sdlEvent);
			}
			ImGui_ImplSDL3_ProcessEvent(&sdlEvent);
		}

		Context->SwapBuffers();
	}

	void SSDLWindow::HandleSDLEvent(SDL_Event const& sdlEvent)
	{
		if (sdlEvent.type >= SDL_EVENT_WINDOW_FIRST && sdlEvent.type <= SDL_EVENT_WINDOW_LAST)
		{
			HandleSDLWindowEvent(sdlEvent.window);
		}

		switch (sdlEvent.type)
		{
			// Mouse Events
			case SDL_EVENT_MOUSE_MOTION:
			{
				WinData.EventCallback(MouseMoveEvent((void*)&sdlEvent, sdlEvent.motion.x, sdlEvent.motion.y));
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				MouseCode code = static_cast<MouseCode>(sdlEvent.button.button);
				WinData.EventCallback(MouseBtnDownEvent((void*)&sdlEvent, code));
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_UP:
			{
				MouseCode code = static_cast<MouseCode>(sdlEvent.button.button);
				WinData.EventCallback(MouseBtnUpEvent((void*)&sdlEvent, code));
				break;
			}
			case SDL_EVENT_MOUSE_WHEEL:
			{
				WinData.EventCallback(MouseScrollEvent((void*)&sdlEvent, sdlEvent.wheel.x, sdlEvent.wheel.y));
				break;
			}
			// Keyboard Events
			case SDL_EVENT_KEY_DOWN:
			{
				KeyCode keyCode = static_cast<KeyCode>(sdlEvent.key.keysym.scancode);
				KeyMod keyMod = static_cast<KeyMod>(sdlEvent.key.keysym.mod);
				WinData.EventCallback(KeyDownEvent((void*)&sdlEvent, keyCode, keyMod));
				break;
			}
			case SDL_EVENT_KEY_UP:
			{
				KeyCode keyCode = static_cast<KeyCode>(sdlEvent.key.keysym.scancode);
				KeyMod keyMod = static_cast<KeyMod>(sdlEvent.key.keysym.mod);
				WinData.EventCallback(KeyUpEvent((void*)&sdlEvent, keyCode, keyMod));
				break;
			}
			default:
			{
				// More events to be implemented
				break;
			}
		}
	}

	void SSDLWindow::HandleSDLWindowEvent(SDL_WindowEvent const& sdlEvent)
	{
		switch (sdlEvent.type)
		{
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				WinData.EventCallback(WinCloseEvent((void*) &sdlEvent));
				break;
			}
			case SDL_EVENT_WINDOW_RESIZED:
			{
				WinData.Width = sdlEvent.data1;
				WinData.Height = sdlEvent.data2;
				WinData.EventCallback(WinResizeEvent((void*)&sdlEvent, WinData.Width, WinData.Height));
				break;
			}
			case SDL_EVENT_WINDOW_MINIMIZED:
			{
				WinData.IsMinimized = true;
				WinData.EventCallback(WinMinimizeEvent((void*)&sdlEvent));
				break;
			}
			case SDL_EVENT_WINDOW_RESTORED:
			{
				WinData.IsMinimized = false;
				WinData.EventCallback(WinRestoreEvent((void*)&sdlEvent));
				break;
			}
			case SDL_EVENT_WINDOW_MAXIMIZED:
			{
				WinData.EventCallback(WinMaximizeEvent((void*)&sdlEvent, true));
				break;
			}
			case SDL_EVENT_WINDOW_FOCUS_GAINED:
			{
				WinData.EventCallback(WinFocusEvent((void*)&sdlEvent));
				break;
			}
			case SDL_EVENT_WINDOW_FOCUS_LOST:
			{
				WinData.EventCallback(WinFocusLostEvent((void*)&sdlEvent));
				break;
			}
			case SDL_EVENT_WINDOW_MOVED:
			{
				WinData.EventCallback(WinMoveEvent((void*)&sdlEvent, sdlEvent.data1, sdlEvent.data2));
				break;
			}
			default:
			{
				// More events to be implemented
				break;
			}
		}
	}

	void SSDLWindow::Shutdown()
	{
		SDL_DestroyWindow(Window);
		sdlWindowCount--;

		if (sdlWindowCount == 0)
		{
			SDL_Quit();
		}
	}
}