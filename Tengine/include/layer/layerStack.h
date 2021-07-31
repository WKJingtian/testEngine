#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
#include "layer.h"
#include <vector>

namespace tengine
{
	class layerStack
	{
	public:
		T_API layerStack();
		T_API ~layerStack();
		void T_API pushLayer(t_layer* l);
		void T_API pushOverlay(t_layer* l);
		void T_API popLayer(t_layer* l);
		void T_API popOverlay(t_layer* l);
		std::vector<t_layer*>::iterator T_API begin() { return lay.begin(); }
		std::vector<t_layer*>::iterator T_API end() { return lay.end(); }
	private:
		std::vector<t_layer*> lay;
		unsigned int insert = 0;
	};
}