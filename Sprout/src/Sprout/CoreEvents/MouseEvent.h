#pragma once

#include "Sprout/Input/MouseCode.h"
#include "CoreEvent.h"

namespace Sprout
{
	class MouseBtnDownEvent : public Event
	{
	public:
		MouseBtnDownEvent(void* nativeEvent, MouseCode btnCode) : Event(nativeEvent), BtnCode(btnCode) {}

		EVENT_CATEGORY_TYPE(EventCategory::Input | EventCategory::Mouse)
		EVENT_CLASS_TYPE(MouseBtnDown)
		
		MouseCode GetMouseCode() { return BtnCode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseBtnDownEvent: " << EnumValue(BtnCode);
			return ss.str();
		}

	private:
		MouseCode BtnCode;
	};

	class MouseBtnUpEvent : public Event
	{
	public:
		MouseBtnUpEvent(void* nativeEvent, MouseCode btnCode) : Event(nativeEvent), BtnCode(btnCode) {}

		EVENT_CATEGORY_TYPE(EventCategory::Input | EventCategory::Mouse)
		EVENT_CLASS_TYPE(MouseBtnUp)

		MouseCode GetMouseCode() { return BtnCode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseBtnUpEvent: " << EnumValue(BtnCode);
			return ss.str();
		}

	private:
		MouseCode BtnCode;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(void* nativeEvent, float x, float y) : Event(nativeEvent), X(x), Y(y) {}

		EVENT_CATEGORY_TYPE(EventCategory::Input | EventCategory::Mouse)
		EVENT_CLASS_TYPE(MouseMove)

		float GetX() const { return X; }
		float GetY() const { return Y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: (" << X << ", " << Y << ")";
			return ss.str();
		}

	private:
		float X, Y;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(void* nativeEvent, float scrollX, float scrollY) : Event(nativeEvent), ScrollX(scrollX), ScrollY(scrollY) {}

		EVENT_CATEGORY_TYPE(EventCategory::Input | EventCategory::Mouse)
		EVENT_CLASS_TYPE(MouseScroll)

		float GetScrollX() const { return ScrollX; }
		float GetScrollY() const { return ScrollY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << ScrollX << ", " << ScrollY;
			return ss.str();
		}

	private:
		float ScrollX, ScrollY;
	};
}