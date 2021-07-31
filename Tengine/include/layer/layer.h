#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
namespace tengine
{
	class t_layer
	{
	public:
		t_layer(const std::string& n = "Layer");
		virtual ~t_layer();
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(float timeStamp) {}
		virtual void onEvent(t_event& e) {}
		virtual void guiRender() {}
		std::string name;
	};
}