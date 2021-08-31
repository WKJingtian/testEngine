#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
#include "camera.h"
#include "layer/layerall.h"
#include "util/utilall.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "bx/bx.h"
#include "bx/debug.h"
#include "imgui.h"
#include "bgfxDraw.h"

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "bigg.h"

namespace tengine
{
	static bgfx::ShaderHandle createShader(const std::string& shader, const char* name)
	{
		const bgfx::Memory* mem = bgfx::copy(shader.data(), (uint32_t)shader.size());
		const bgfx::ShaderHandle handle = bgfx::createShader(mem);
		bgfx::setName(handle, name);
		return handle;
	}
	class editorApp : public bigg::Application
	{
		static editorApp* editor;

		void initialize(int _argc, char** _argv);

		void onReset();

		void update(float dt);

		void pushLayer(t_layer* l);

		int vTest = 0;
	public:
		// ATTRIBUTES
		layerStack ls;
		bgfx::VertexBufferHandle bgfxVertexBuffer;
		bgfx::IndexBufferHandle bgfxIndexBuffer;
		PosColorVertex vertexes[1024];
		uint32_t indexes[4096];
		bgfx::ProgramHandle program;
		int width = 1280, height = 720;
		bool running = true;

		editorApp()
			: bigg::Application("Editor") {}
		static inline editorApp* getApp() { return editor; }
		inline GLFWwindow* getWin() { return mWindow; }

	};
}