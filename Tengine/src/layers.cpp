#include "tpch.h"
#include "layer/layerall.h"

#include "imgui.h"
#include "application.h"
#include "tengine_core.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

#include "input.h"

#include "imgui.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

namespace tengine
{
	// layer.h
	t_layer::t_layer(const std::string& n) : name(n) {}
	t_layer::~t_layer() {}

	// layerStack.h
	layerStack::layerStack() { }
	layerStack::~layerStack()
	{
		for (t_layer* l : lay) delete l;
	}
	void layerStack::pushLayer(t_layer* l)
	{
		lay.emplace(lay.begin() + insert, l);
		insert++;
	}
	void layerStack::pushOverlay(t_layer* l)
	{
		lay.emplace_back(l);
	}
	void layerStack::popLayer(t_layer* l)
	{
		auto it = std::find(lay.begin(), lay.end(), l);
		if (it != lay.end())
		{
			lay.erase(it);
			insert--;
		}
	}
	void layerStack::popOverlay(t_layer* l)
	{
		auto it = std::find(lay.begin(), lay.end(), l);
		if (it != lay.end())
		{
			lay.erase(it);
		}
	}
}