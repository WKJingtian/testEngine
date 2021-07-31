#pragma once
#include "tpch.h"
#include "tengine_core.h"
#include "event/eventall.h"

namespace tengine
{
	struct windowProp
	{
		std::string title;
		unsigned int width, height;
		windowProp(const std::string& t = "T-engine\0",
			unsigned int w = 1280, unsigned int h = 720)
			: title(t), width(w), height(h) {}
	};

	class t_window
	{
	public:
		using eventCallbackFn = std::function<void(t_event&)>;
		virtual ~t_window() {}
		virtual void onUpdate() = 0;
		virtual inline unsigned int getWidth() const  = 0;
		virtual inline unsigned int getHeight() const = 0;
		virtual void setEventCallback(const eventCallbackFn& callback) = 0;
		virtual void setVSync(bool enable) = 0;
		virtual bool isVSync() const = 0;
		virtual void* getWindow() const = 0;
		static t_window* create(const windowProp& props = windowProp());
	};
}