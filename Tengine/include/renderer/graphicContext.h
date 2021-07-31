#pragma once
#include "tengine_core.h"

struct GLFWwindow;
namespace tengine
{
	class graphicContext
	{
	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};
}