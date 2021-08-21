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
		layerStack();
		~layerStack();
		void pushLayer(t_layer* l);
		void pushOverlay(t_layer* l);
		void popLayer(t_layer* l);
		void popOverlay(t_layer* l);
		std::vector<t_layer*>::iterator begin() { return lay.begin(); }
		std::vector<t_layer*>::iterator end() { return lay.end(); }
	private:
		std::vector<t_layer*> lay;
		unsigned int insert = 0;
	};
}