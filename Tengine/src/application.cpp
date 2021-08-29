#include "tpch.h"
#include "application.h"
#include "editor2d.h"

namespace tengine
{
	editorApp* editorApp::editor = 0;

	void editorApp::initialize(int _argc, char** _argv)
	{
		if (editor == nullptr) editor = this;
		else errout("multiple editor instance created");
		running = true;
		pushLayer(new editor2d());

		std::string vshader;
		if (!read_file("./asset/shader/bgfxVertex.bin", vshader))
			errout("vertex shader compile failed");

		std::string fshader;
		if (!read_file("./asset/shader/bgfxFrag.bin", fshader))
			errout("fragment shader compile failed");

		bgfx::ShaderHandle vShader = createShader(vshader, "bgfx vertex shader");
		bgfx::ShaderHandle fShader = createShader(fshader, "bgfx fragment shader");

		program = bgfx::createProgram(vShader, fShader, true);
		//bgfx::renderFrame();
	}

	void editorApp::onReset()
	{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000055FF, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
	}

	void editorApp::update(float dt)
	{
		//if (!running) exit(0);
		memset(vertexes, 0, 1024 * sizeof(PosColorVertex));
		memset(indexes, 0, 4096 * sizeof(uint32_t));
		bgfx::VertexLayout layout = bgfx::VertexLayout();
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
		bgfxVertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(vertexes, 1024 * sizeof(PosColorVertex)), layout);
		bgfxIndexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(indexes, 4096 * sizeof(uint32_t)));

		bgfx::touch(0);
		for (t_layer* l : ls) l->onUpdate(dt);
		for (t_layer* l : ls) l->guiRender();

		static glm::mat4 originP = glm::mat4(1);
		bgfx::setTransform(&originP);
		bgfx::setVertexBuffer(0, bgfxVertexBuffer);
		bgfx::setIndexBuffer(bgfxIndexBuffer);
		bgfx::submit(0, program);
	}

	void editorApp::pushLayer(t_layer* l)
	{
		ls.pushLayer(l);
		l->onAttach();
	}
}