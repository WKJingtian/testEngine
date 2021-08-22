#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
#include "graphic/graphicall.h"
#include "layer/layerall.h"
#include "util/utilall.h"

#include "bgfxImgui.h"
#include "sdlImgui.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "imgui.h"
#include "bgfxDraw.h"
#include "imgui.h"

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"


namespace tengine
{
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	class application
	{
	public:
		application();
		virtual ~application();
		void run();
		void onEvent(t_event& e);
		void pushLayer(t_layer* l);
		void pushOverlay(t_layer* l);
		static inline application& getApp() { return *app_instance; }
		static inline float getDelta() { return deltaTime; }
	private:
		static float deltaTime;
		static timeStamp ts;
		static application* app_instance;

		bool onClose(windowCloseEvent& e);
		bool onResized(windowResizeEvent& e);
		bool running;
		bool minimize = false;
		layerStack ls;
		imguiLayer* guiLayer;

	public:
		bgfx::Init bgfxInit;
		SDL_Window* sdlWindow;
		GLFWwindow* glfwWindow;
		int width = 1280, height = 720;
		bgfx::VertexBufferHandle bgfxVertexBuffer;
		bgfx::IndexBufferHandle bgfxIndexBuffer;
		bgfx::ProgramHandle program;

		PosColorVertex vertexes[1024];
		uint32_t indexes[2096];
	};
	application* createApplication();
	static bgfx::ShaderHandle createShader(const std::string& shader, const char* name)
	{
		const bgfx::Memory* mem = bgfx::copy(shader.data(), (uint32_t)shader.size());
		const bgfx::ShaderHandle handle = bgfx::createShader(mem);
		bgfx::setName(handle, name);
		return handle;
	}
}