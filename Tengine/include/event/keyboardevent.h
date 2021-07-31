#pragma once
#include "event.h"

namespace tengine
{
	class T_API keyEvent : public t_event
	{
	public:
		inline int getKeycode() const{ return keycode; }
		EVENT_CLASS_CATEGORY(ecKeyboard | ecInput)
	protected:
		keyEvent(int kc)
			: keycode(kc) {}
		int keycode;
	};

	class T_API keyPressedEvent : public keyEvent
	{
	protected:
		int repeatCount;
	public:
		inline int getRepeat() const { return repeatCount; }
		keyPressedEvent(int kc, int rc)
			: keyEvent(kc), repeatCount(rc) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyPressedEvent: keycode->" << keycode << "; repeated->" << repeatCount;
			return ss.str();
		}
		EVENT_CLASS_TYPE(keyReleased)
	};

	class T_API keyTypedEvent : public keyEvent
	{
	public:
		keyTypedEvent(int kc)
			: keyEvent(kc) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyTypedEvent: keycode->" << keycode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(keyTyped)
	};

	class T_API keyReleasedEvent : public keyEvent
	{
	public:
		keyReleasedEvent(int kc)
			: keyEvent(kc) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyReleasedEvent: keycode->" << keycode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(keyReleased)
	};
}