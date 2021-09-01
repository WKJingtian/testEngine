#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
#include "layer.h"
#include "util/utilall.h"
#include <vector>

namespace tengine
{
	class layerStack
	{
	public:
		layerStack();
		~layerStack();
		void pushLayer(ownedPtr<t_layer> l);
		void pushOverlay(ownedPtr<t_layer> l);
		void popLayer(ownedPtr<t_layer> l);
		void popOverlay(ownedPtr<t_layer> l);
		std::vector<ownedPtr<t_layer>>::iterator begin() { return lay.begin(); }
		std::vector<ownedPtr<t_layer>>::iterator end() { return lay.end(); }
	private:
		std::vector<ownedPtr<t_layer>> lay;
		unsigned int insert = 0;
	};
}