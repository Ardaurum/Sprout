#pragma once

#include "Sprout/CoreEvents/CoreEvent.h"

namespace Sprout
{
	class WinResizeEvent : public Event
	{
	public:
		WinResizeEvent(uint32_t width, uint32_t height) : Width(width), Height(height) {}

		uint32_t GetWidth() const { return Width; }
		uint32_t GetHeight() const { return Height; }

		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinResize)

	private:
		uint32_t Width, Height;
	};

	class WinCloseEvent : public Event
	{
	public:
		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinClose)
	};

	class WinMinimizeEvent : public Event
	{
	public:
		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinMinimize)
	};
	
	class WinRestoreEvent : public Event
	{
	public:
		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinRestore)
	};

	class WinMaximizeEvent : public Event
	{
	public:
		WinMaximizeEvent(bool maximized) : IsMaximized(maximized) {}

		bool GetIsMaximized() const { return IsMaximized; }

		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinMaximize)

	private:
		bool IsMaximized;
	};

	class WinFocusEvent : public Event
	{
	public:
		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinFocus)
	};

	class WinFocusLostEvent : public Event
	{
	public:
		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinFocusLost)
	};

	class WinMoveEvent : public Event
	{
	public:
		WinMoveEvent(uint32_t x, uint32_t y) : X(x), Y(y) {}

		uint32_t GetX() { return X; }
		uint32_t GetY() { return Y; }

		EVENT_CATEGORY_TYPE(EventCategory::Window)
		EVENT_CLASS_TYPE(WinMove)

	private:
		uint32_t X, Y;
	};
}