#include "tpch.h"
#include "input.h"
#include "glfw/glfw3.h"
#include "application.h"
#include "graphic/graphicall.h"

namespace tengine
{
	input* input::input_instance = new winInput();

	bool winInput::isKeyPressedImpl(int keycode)
	{
		auto win = static_cast<GLFWwindow*>(application::getApp().getwin().getWindow());
		auto state = glfwGetKey(win, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool winInput::isButtonPressedImpl(int button)
	{
		auto win = static_cast<GLFWwindow*>(application::getApp().getwin().getWindow());
		auto state = glfwGetMouseButton(win, button);
		return state == GLFW_PRESS;
	}
	float winInput::getmxImpl()
	{
		auto win = static_cast<GLFWwindow*>(application::getApp().getwin().getWindow());
		double xx, yy;
		glfwGetCursorPos(win, &xx, &yy);
		return (float)xx;
	}
	float winInput::getmyImpl()
	{
		auto win = static_cast<GLFWwindow*>(application::getApp().getwin().getWindow());
		double xx, yy;
		glfwGetCursorPos(win, &xx, &yy);
		return (float)yy;
	}
	std::pair<float, float> winInput::getmPosImpl()
	{
		auto win = static_cast<GLFWwindow*>(application::getApp().getwin().getWindow());
		double xx, yy;
		glfwGetCursorPos(win, &xx, &yy);
		return std::pair<float, float> ((float)xx, (float)yy);
	}
}