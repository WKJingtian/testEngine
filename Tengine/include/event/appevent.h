#pragma once
#include "event.h"

namespace tengine
{
	class T_API windowCloseEvent : public t_event
	{
	public:
		windowCloseEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(windowClose)
	};

	class T_API windowResizeEvent : public t_event
	{
	public:
		windowResizeEvent(int w, int h)
			: width(w), height(h) {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(windowResize)
			std::string toString() const override
		{
			std::stringstream ss;
			ss << "windowResizeEvent: width->" << width << "; height->" << height;
			return ss.str();
		}
		unsigned int width, height;
	};

	class T_API appTickEvent : public t_event
	{
	public:
		appTickEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(appTick)
	};

	class T_API appUpdateEvent : public t_event
	{
	public:
		appUpdateEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(appUpdate)
	};

	class T_API appRenderEvent : public t_event
	{
	public:
		appRenderEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(appRender)
	};
}