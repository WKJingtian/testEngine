#pragma once
#include "tpch.h"
#include "tengine_core.h"

namespace tengine
{
	enum class eventType
	{
		None = 0,
		windowClose, windowFocus, windowLostFocus, windowMoved, windowResize,
		appTick, appUpdate, appRender,
		keyPressed, keyReleased, keyTyped,
		mousePressed, mouseReleased, mouseMoved, mouseScrolled
	};

	enum eventCategory
	{
		None = 0,
		ecApplication = BIT(0),
		ecInput = BIT(1),
		ecKeyboard = BIT(2),
		ecMouse = BIT(3),
		ecMousebutton = BIT(4)
	};

#define EVENT_CLASS_TYPE(x) static eventType getStaticType() {return eventType::##x; }\
	virtual eventType getEventType() const override {return getStaticType(); }\
	virtual const char* getName() const override {return #x; }

#define EVENT_CLASS_CATEGORY(x) virtual int getCategoryFlags() const override {return x;}

	class t_event
	{
		friend class t_dispatcher;
	public:
		virtual eventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }
		inline bool isCategory(eventCategory ec) { return getCategoryFlags() & ec;  }
		bool handeled = false;
	};

	class t_dispatcher
	{
		template<typename T>
		using eventFn = std::function<bool(T&)>;
	public:
		t_dispatcher(t_event& e)
			: eve(e) {}
		template<typename T>
		bool dispatch(eventFn<T> func)
		{
			if (eve.getEventType() == T::getStaticType())
			{
				eve.handeled = func(*(T*)&eve);
				return true;
			}
			return false;
		}
	private:
		t_event& eve;
	};

	inline std::ostream& operator<<(std::ostream& os, const t_event& e)
	{
		return os << e.toString();
	}
}
