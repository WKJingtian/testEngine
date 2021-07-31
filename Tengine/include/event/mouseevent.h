#pragma once
#include "event.h"

namespace tengine
{
	class T_API mouseMoveEvent : public t_event
	{
	public:
		mouseMoveEvent(float x, float y)
			: mousex(x), mousey(y) {}
		inline float getx() { return mousex; }
		inline float gety() { return mousey; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "mouseMoveEvent: X->" << mousex << "; Y->" << mousey;
			return ss.str();
		}
		EVENT_CLASS_TYPE(mouseMoved)
		EVENT_CLASS_CATEGORY(ecMouse | ecInput)
	private:
		float mousex, mousey;
	};

	class T_API mouseScrollEvent : public t_event
	{
	public:
		mouseScrollEvent(float x, float y)
			: xoffset(x), yoffset(y) {}
		inline float getx() { return xoffset; }
		inline float gety() { return yoffset; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "mouseScrollEvent: X->" << xoffset << "; Y->" << yoffset;
			return ss.str();
		}
		EVENT_CLASS_TYPE(mouseScrolled)
		EVENT_CLASS_CATEGORY(ecMouse | ecInput)
	private:
		float xoffset, yoffset;
	};

	class T_API buttonEvent : public t_event
	{
	public:
		inline int getButton() const { return button; }
		EVENT_CLASS_CATEGORY(ecMouse | ecInput)
	protected:
		buttonEvent(int b) : button(b) {}
		int button;
	};

	class T_API buttonPressedEvent : public buttonEvent
	{
	public:
		buttonPressedEvent(int b) : buttonEvent(b) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "buttonPressedEvent: button->" << button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(mousePressed)
	};

	class T_API buttonReleasedEvent : public buttonEvent
	{
	public:
		buttonReleasedEvent(int b) : buttonEvent(b) {}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "buttonReleasedEvent: button->" << button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(mouseReleased)
	};
}