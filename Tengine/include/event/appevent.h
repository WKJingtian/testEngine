#pragma once
#include "event.h"

namespace tengine
{
	class windowCloseEvent : public t_event
	{
	public:
		windowCloseEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(windowClose)
	};

	class windowResizeEvent : public t_event
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

	class appTickEvent : public t_event
	{
	public:
		appTickEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(appTick)
	};

	class appUpdateEvent : public t_event
	{
	public:
		appUpdateEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(appUpdate)
	};

	class appRenderEvent : public t_event
	{
	public:
		appRenderEvent() {}
		EVENT_CLASS_CATEGORY(ecApplication)
		EVENT_CLASS_TYPE(appRender)
	};
}