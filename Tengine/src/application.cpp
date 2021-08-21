#include "tpch.h"
#include "application.h"
#include "editor2d.h"

namespace tengine
{
	application* application::app_instance = NULL;
	float application::deltaTime = 0;
	timeStamp application::ts = timeStamp();

	application::application()
	{
		if (app_instance == NULL) app_instance = this;
		else
		{
			log("multiple applications generated!", 3);
			exit(EXIT_FAILURE);
		}

		// Setup window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) errout("glfw start failed, exit...");
		// Create window with graphics context
		glfwWindow = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
		if (glfwWindow == NULL) errout("glfw window start failed, exit...");

		bgfx::renderFrame(); // single threaded mode

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		bgfxInit.platformData.ndt = glfwGetX11Display();
		bgfxInit.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
		bgfxInit.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
		bgfxInit.platformData.nwh = glfwGetWin32Window(glfwWindow);
#endif
		glfwGetWindowSize(glfwWindow, &width, &height);
		bgfxInit.type = bgfx::RendererType::Count; // auto choose renderer
		bgfxInit.resolution.width = width;
		bgfxInit.resolution.height = height;
		bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(bgfxInit);
		
		bgfx::setViewClear(
			0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x112233FF, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, width, height);

		log("making layers...");
		guiLayer = new imguiLayer();
		pushOverlay(guiLayer);
		pushLayer(new editor2d());
		running = true;
		log("layer added!");

		bgfx::VertexLayout layout = bgfx::VertexLayout();
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
		bgfxVertexBuffer = bgfx::createVertexBuffer(
			bgfx::makeRef(vertexes, 1024),
			layout);
		bgfxIndexBuffer = bgfx::createIndexBuffer(
			bgfx::makeRef(indexes, 4096));

		std::string vshader;
		if (!read_file("./asset/shader/bgfxVertex.bin", vshader))
			errout("vertex shader compile failed");

		std::string fshader;
		if (!read_file("./asset/shader/bgfxFrag.bin", fshader))
			errout("fragment shader compile failed");

		bgfx::ShaderHandle vShader = createShader(vshader, "bgfx vertex shader");
		bgfx::ShaderHandle fShader = createShader(fshader, "bgfx fragment shader");

		program = bgfx::createProgram(vShader, fShader, true);
	}

	application::~application()
	{
		bgfx::destroy(bgfxVertexBuffer);
		bgfx::destroy(bgfxIndexBuffer);
		bgfx::destroy(program);
		ImGui::DestroyContext();
		bgfx::shutdown();
	}

	void application::onEvent(t_event& e)
	{
		t_dispatcher disp(e);
		disp.dispatch<windowCloseEvent>(BIND_EVENT(application::onClose));
		disp.dispatch<windowResizeEvent>(BIND_EVENT(application::onResized));
		for (auto l = ls.end(); l != ls.begin(); )
		{
			(*--l)->onEvent(e);
			if (e.handeled) break;
		}
	}

	bool application::onClose(windowCloseEvent& e)
	{
		running = false;
		return true;
	}
	bool application::onResized(windowResizeEvent& e)
	{
		if (e.height == 0 || e.width == 0) minimize = true;
		else minimize = false;
		renderer::onResized(e.width, e.height);
		return false;
	}

	void application::run()
	{
		ts.init();
		bool swi = false;
		while (running)
		{
			glfwPollEvents();

			// report how long this frame take
			deltaTime = (float)ts.reportDelta();
			//log("running...")
			
			// render graphic and gui
			for (t_layer* l : ls) l->onUpdate(deltaTime);
			guiLayer->begin();
			for (t_layer* l : ls) l->guiRender();
			guiLayer->end();

			bgfx::setVertexBuffer(0, bgfxVertexBuffer);
			bgfx::setIndexBuffer(bgfxIndexBuffer);
			bgfx::submit(0, program);
			bgfx::frame();
			if (input::isButtonPressed(0))
			{
				log("switch mode...");
				swi = !swi;
			}
		}
	}

	void application::pushLayer(t_layer* l)
	{
		ls.pushLayer(l);
		l->onAttach();
	}

	void application::pushOverlay(t_layer* l)
	{
		ls.pushOverlay(l);
		l->onAttach();
	}
}