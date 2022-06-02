#pragma once

#include "CoreEvent.h"
#include "Sprout/Input/KeyCode.h"
#include <sstream>

namespace Sprout
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() { return Key; }

		EVENT_CATEGORY_TYPE(EventCategory::Input | EventCategory::Keyboard)

	protected:
		KeyEvent(const KeyCode key) : Key(key) {}

		KeyCode Key;
	};

	class KeyModEvent : public KeyEvent
	{
	public:
		KeyMod GetKeyMod() { return Mod; }

	protected:
		KeyModEvent(const KeyCode key, const KeyMod mod) : KeyEvent(key), Mod(mod) {}

		KeyMod Mod;
	};

	class KeyDownEvent : public KeyModEvent
	{
	public:
		KeyDownEvent(const KeyCode key, const KeyMod mod) : KeyModEvent(key, mod) {};

		EVENT_CLASS_TYPE(KeyDown)

		std::string ToString() const override
		{ 
			std::stringstream ss;
			ss << "KeyDownEvent: " << EnumValue(Key) << " Mod: " << EnumValue(Mod);
			return ss.str(); 
		}
	};

	class KeyRepeatEvent : public KeyModEvent
	{
	public:
		KeyRepeatEvent(const KeyCode key, const KeyMod mod) : KeyModEvent(key, mod) {};

		EVENT_CLASS_TYPE(KeyRepeat)

		std::string ToString() const override
		{ 
			std::stringstream ss;
			ss << "KeyRepeatEvent: " << EnumValue(Key) << " Mod: " << EnumValue(Mod);
			return ss.str(); 
		}
	};

	class KeyUpEvent : public KeyModEvent
	{
	public:
		KeyUpEvent(const KeyCode key, const KeyMod mod) : KeyModEvent(key, mod) {};

		EVENT_CLASS_TYPE(KeyUp)

		std::string ToString() const override
		{ 
			std::stringstream ss;
			ss << "KeyUpEvent: " << EnumValue(Key) << " Mod: " << EnumValue(Mod);
			return ss.str(); 
		}
	};

	class KeyTypeEvent : public KeyEvent
	{
	public:
		KeyTypeEvent(const KeyCode key) : KeyEvent(key) {};

		EVENT_CLASS_TYPE(KeyType)

		std::string ToString() const override
		{ 
			std::stringstream ss;
			ss << "KeyTypeEvent: " << EnumValue(Key);
			return ss.str(); 
		}
	};
}