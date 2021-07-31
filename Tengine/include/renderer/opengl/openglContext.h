#pragma once
#include "../graphicContext.h"

namespace tengine
{
	class openglContext : public graphicContext
	{
	public:
		openglContext(GLFWwindow* w);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* win;
	};
}