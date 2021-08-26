#include "tpch.h"
#include "input.h"
#include "application.h"
#include "graphic/graphicall.h"
#include "sdlImgui.h"

namespace tengine
{
	input* input::input_instance = new winInput();

	bool winInput::isKeyPressedImpl(int keycode)
	{
		auto state = glfwGetKey(editorApp::getApp()->getWin(), keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool winInput::isButtonPressedImpl(uint32_t button)
	{
		auto state = glfwGetMouseButton(editorApp::getApp()->getWin(), button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	float winInput::getmxImpl()
	{
		double xx, yy;
		glfwGetCursorPos(editorApp::getApp()->getWin(), &xx, &yy);
		return (float)xx;
	}
	float winInput::getmyImpl()
	{
		double xx, yy;
		glfwGetCursorPos(editorApp::getApp()->getWin(), &xx, &yy);
		return (float)yy;
	}
	std::pair<float, float> winInput::getmPosImpl()
	{
		double xx, yy;
		glfwGetCursorPos(editorApp::getApp()->getWin(), &xx, &yy);
		return std::pair<float, float> ((float)xx, (float)yy);
	}
}