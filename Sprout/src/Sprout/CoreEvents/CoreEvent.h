#pragma once

#include "Sprout/Core/Core.h"
#include "Sprout/Core/EnumUtils.h"

namespace Sprout
{
	enum class EventType
	{
		WinClose, WinResize, WinMinimize, WinRestore, WinMaximize, WinFocus, WinFocusLost, WinMove,
		MouseBtnDown, MouseBtnUp, MouseMove, MouseScroll,
		KeyDown, KeyRepeat, KeyUp, KeyType
	};

	enum class EventCategory
	{
		None = 0,
		Window = 1 << 0,
		Input = 1 << 1,
		Keyboard = 1 << 2,
		Mouse = 1 << 3
	};

	MAKE_ENUM_FLAG(EventCategory)

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
									virtual EventType GetType() const override { return GetStaticType(); } \
									virtual const char* GetName() const override { return #type; }

	#define EVENT_CATEGORY_TYPE(category) virtual EventCategory GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		Event(void* nativeEv) : NativeEvent(nativeEv) {}
		virtual ~Event() = default;

		virtual EventCategory GetCategoryFlags() const = 0;
		virtual const char* GetName() const = 0;
		virtual EventType GetType() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory flags) const noexcept
		{
			return Any(flags & GetCategoryFlags());
		}

		bool IsHandled() const { return Handled; }
		void SetHandled() noexcept { Handled = true; };
		void* GetNativeEvent() const { return NativeEvent; }

	private:
		void* NativeEvent = nullptr;
		bool Handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& eventData) : EventData(eventData) {}

		template<typename T, typename F>
		bool Dispatch(const F& fn)
		{
			if (EventData.GetType() == T::GetStaticType())
			{
				if (fn(static_cast<T&>(EventData)))
				{
					EventData.SetHandled();
				}
				return true;
			}
			return false;
		}

	private:
		Event& EventData;
	};

	static std::ostream& operator<<(std::ostream& os, const Event& ev)
	{
		return os << ev.ToString();
	}
}