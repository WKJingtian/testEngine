#include "tpch.h"
#include "graphic/graphicall.h"
#include "util/utillog.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "input.h"

namespace tengine
{
	static bool GLFW_initialized = false;
//	t_window* t_window::create(const windowProp& props) { return new winWindow(props); }

	void glfwErrorCallback(int e, const char* msg) { log(msg, 4); }
//	winWindow::winWindow(const windowProp& p) { init(p); }
//	winWindow::~winWindow() { shutdown(); }
//	void winWindow::init(const windowProp& p)
//	{
//		prop.title = p.title; prop.width = p.width; prop.height = p.height;
//		if (!GLFW_initialized)
//		{
//			glfwSetErrorCallback(glfwErrorCallback);
//			if (glfwInit())
//				log("glfw starts successfully", 0);
//			else
//			{
//				log("glfw initialization failed", 3);
//				glfwTerminate();
//				exit(EXIT_FAILURE);
//			}
//			GLFW_initialized = true;
//		}
//		win = glfwCreateWindow((int)prop.width, (int)prop.height, prop.title.c_str(), NULL, NULL);
//		context = new openglContext(win);
//		context->init();
//
//		glfwSetWindowUserPointer(win, &prop);
//		setVSync(true);
//
//		glfwSetWindowSizeCallback(win, [](GLFWwindow* window, int width, int height)
//			{
//				win_data* w = (win_data*)glfwGetWindowUserPointer(window);
//				w->height = height;
//				w->width = width;
//				windowResizeEvent eve(width, height);
//				w->eventCallback(eve);
//			});
//		glfwSetWindowCloseCallback(win, [](GLFWwindow* window)
//			{
//				win_data* w = (win_data*)glfwGetWindowUserPointer(window);
//				windowCloseEvent eve;
//				w->eventCallback(eve);
//			});
//		glfwSetCharCallback(win, [](GLFWwindow* window, unsigned int character)
//			{
//				win_data* w = (win_data*)glfwGetWindowUserPointer(window);
//				keyTypedEvent eve(character);
//				w->eventCallback(eve);
//			});
//		glfwSetKeyCallback(win, [](GLFWwindow* window, int key, int scancode, int action, int mods)
//			{
//				win_data* w = (win_data*)glfwGetWindowUserPointer(window);
//				switch (action)
//				{
//					case GLFW_PRESS:
//					{
//						keyPressedEvent eve(key, 0);
//						w->eventCallback(eve);
//						break;
//					}
//					case GLFW_RELEASE:
//					{
//						keyReleasedEvent eve(key);
//						w->eventCallback(eve);
//						break;
//					}
//					case GLFW_REPEAT:
//					{
//						keyPressedEvent eve(key, 1);
//						w->eventCallback(eve);
//						break;
//					}
//				}
//			});
//		glfwSetMouseButtonCallback(win, [](GLFWwindow* window, int button, int action, int mods)
//			{
//				win_data* w = (win_data*)glfwGetWindowUserPointer(window);
//				switch (action)
//				{
//					case GLFW_PRESS:
//					{
//						buttonPressedEvent eve(button);
//						w->eventCallback(eve);
//						break;
//					}
//					case GLFW_RELEASE:
//					{
//						buttonReleasedEvent eve(button);
//						w->eventCallback(eve);
//						break;
//					}
//					case GLFW_REPEAT:
//					{
//						buttonPressedEvent eve(button);
//						w->eventCallback(eve);
//						break;
//					}
//				}
//			});
//		glfwSetScrollCallback(win, [](GLFWwindow* window, double xOffset, double yOffset)
//			{
//				win_data * w = (win_data*)glfwGetWindowUserPointer(window);
//				mouseScrollEvent eve((float)xOffset, (float)yOffset);
//				w->eventCallback(eve);
//			});
//		glfwSetCursorPosCallback(win, [](GLFWwindow* window, double xPos, double yPos)
//			{
//				win_data* w = (win_data*)glfwGetWindowUserPointer(window);
//				mouseMoveEvent eve((float)xPos, (float)yPos);
//				w->eventCallback(eve);
//			});
//	}
//	void winWindow::shutdown() { glfwDestroyWindow(win); }
//	void winWindow::onUpdate()
//	{
//		glfwPollEvents();
//		context->swapBuffers();
//	}
//	void winWindow::setVSync(bool enable)
//	{
//		if (enable) glfwSwapInterval(1);
//		else glfwSwapInterval(0);
//		prop.VSync = enable;
//	}
//	bool winWindow::isVSync() const { return prop.VSync; }

	// camera
	void sceneCamera::setOrthographic(float size, float n, float f)
	{
		type = sceneCamera::camType::orthographic;
		orthographicSize = size;
		orthographicNear = n;
		orthographicFar = f;
		recalculate();
	}
	void sceneCamera::setPerspective(float FOV, float n, float f)
	{
		type = sceneCamera::camType::perspective;
		perspectiveFOV = FOV;
		perspectiveNear = n;
		perspectiveFar = f;
		recalculate();
	}
	void sceneCamera::setViewportSize(uint32_t w, uint32_t h)
	{
		aspectRatio = (float)w / (float)h;
		recalculate();
	}
	void sceneCamera::recalculate()
	{
		//projection = glm::ortho(-r * z, r * z, -z, z, -1.0f, 1.0f);
		if (aspectRatio == 0)
		{
			log("invalid aspect ratio detected (0)", 2);
			return;
		}
		if (type == camType::orthographic)
		{
			float left = -aspectRatio * orthographicSize * 0.5f;
			float right = aspectRatio * orthographicSize * 0.5f;
			float bottom = -orthographicSize * 0.5f;
			float top = orthographicSize * 0.5f;
			projection = glm::ortho(left, right, bottom, top, orthographicNear, orthographicFar);
		}
		if (type == camType::perspective)
		{
			projection = glm::perspective(glm::radians(perspectiveFOV), aspectRatio, perspectiveNear, perspectiveFar);
		}
	}

	orthographicCamera::orthographicCamera(float l, float r, float b, float t)
		: project(glm::ortho(l, r, b, t, -1.0f, 1.0f)), view(1)
	{
		viewProject = project * view;
	}

	void orthographicCamera::recalculate()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0,0,1));

		view = glm::inverse(transform);
		viewProject = project * view;
	}
	void orthographicCamera::setZoom(float r, float z)
	{ 
		project = glm::ortho(-r * z, r * z, -z, z, -1.0f, 1.0f);
		recalculate();
	}
	void orthographicCamera::setProject(float l, float r, float b, float t)
	{
		project = glm::ortho(l, r, b, t, -1.0f, 1.0f);
		recalculate();
	}

	cameraControl::cameraControl(float r, float z = 1)
		:cam(-r*z, r*z, -z, z), aspectRatio(r), zoomLevel(z)
	{

	}
	void cameraControl::onUpdate(float deltaTime)
	{
		dtime = deltaTime;
	}
	void cameraControl::onEvent(t_event& e)
	{
		t_dispatcher dispatcher(e);
		dispatcher.dispatch<keyPressedEvent>(BIND_EVENT(cameraControl::onPressed));
		dispatcher.dispatch<mouseScrollEvent>(BIND_EVENT(cameraControl::onScrolled));
		dispatcher.dispatch<windowResizeEvent>(BIND_EVENT(cameraControl::onResized));
		dispatcher.dispatch<keyTypedEvent>(BIND_EVENT(cameraControl::onTyped));
		dispatcher.dispatch<buttonPressedEvent>(BIND_EVENT(cameraControl::onButton));
	}
	bool cameraControl::onScrolled(mouseScrollEvent& e)
	{
		if (e.gety() > 0)
			cam.setRot(cam.rot + 1);
		if (e.gety() < 0)
			cam.setRot(cam.rot - 1);
		return false;
	}
	bool cameraControl::onResized(windowResizeEvent& e)
	{
		float rat = (float)e.width / (float)e.height;
		cam.setProject(-rat * zoomLevel, rat * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
	bool cameraControl::onPressed(keyPressedEvent& e)
	{
		if (input::isKeyPressed(T_KEY_W))
			cam.setPos(cam.pos + glm::vec3(forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_A))
			cam.setPos(cam.pos + glm::vec3(-rightVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_S))
			cam.setPos(cam.pos + glm::vec3(-forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_D))
			cam.setPos(cam.pos + glm::vec3(rightVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_UP))
			cam.setPos(cam.pos + glm::vec3(forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_LEFT))
			cam.setPos(cam.pos + glm::vec3(-rightVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_DOWN))
			cam.setPos(cam.pos + glm::vec3(-forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_RIGHT))
			cam.setPos(cam.pos + glm::vec3(rightVec(dtime) / 1000000.0f, 0));
		return false;
	}
	bool cameraControl::onTyped(keyTypedEvent& e)
	{
		if (input::isKeyPressed(T_KEY_W))
			cam.setPos(cam.pos + glm::vec3(forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_A))
			cam.setPos(cam.pos + glm::vec3(-rightVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_S))
			cam.setPos(cam.pos + glm::vec3(-forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_D))
			cam.setPos(cam.pos + glm::vec3(rightVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_UP))
			cam.setPos(cam.pos + glm::vec3(forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_LEFT))
			cam.setPos(cam.pos + glm::vec3(-rightVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_DOWN))
			cam.setPos(cam.pos + glm::vec3(-forwardVec(dtime) / 1000000.0f, 0));
		if (input::isKeyPressed(T_KEY_RIGHT))
			cam.setPos(cam.pos + glm::vec3(rightVec(dtime) / 1000000.0f, 0));
		return false;
	}
	bool cameraControl::onButton(buttonPressedEvent& e)
	{
		if (e.getButton() == T_MOUSE_BUTTON_1) zoomLevel += 1.5f;
		if (e.getButton() == T_MOUSE_BUTTON_2) zoomLevel -= 1.5f;
		if (e.getButton() == T_MOUSE_BUTTON_3) zoomLevel = 10;
		cam.setZoom(aspectRatio, zoomLevel);
		return false;
	}
	glm::vec2 cameraControl::rightVec(float deltaTime)
	{
		float ang = cam.rot;
		return glm::vec2(cos(glm::radians(ang)), sin(glm::radians(ang))) * speed * deltaTime;
	}

	glm::vec2 cameraControl::forwardVec(float deltaTime)
	{
		float ang = cam.rot;
		return glm::vec2(-sin(glm::radians(ang)), cos(glm::radians(ang))) * speed * deltaTime;
	}

}