#pragma once

#include "Sprout/CoreEvents/CoreEvent.h"

namespace Sprout
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(const std::string& title = "Sprout Engine", uint32_t width = 1280, uint32_t height = 800):
			Title(title), Width(width), Height(height)
		{ }
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Update() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual float GetAspect() const = 0;

		virtual void SetCallbackFn(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void* GetRendererContext() const = 0;
		static std::shared_ptr<Window> Create(const WindowProperties& properties = WindowProperties());
	};
}