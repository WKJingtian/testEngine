#include "editor2d.h"
#include "imgui.h"
#include "tengine_core.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

editor2d::editor2d()
{
	// initialize graphic stuff
	// renderCommand::init();
	// renderer2D::init();
	// renderer3D::init();

	// profiling
	// profiles.push_back(profileTime(0, "on Update"));
}

editor2d::~editor2d()
{
	//delete fb;
}

void editor2d::onAttach()
{
	// frame buffer
	// frameBufferProp p;
	// p.width = 1280;
	// p.height = 720;
	// fb = frameBuffer::create(p);

	// entity system
	activeScene = new scene();
	cameraEntity = activeScene->createEntity();
	cameraEntity->addComponent<cameraComponent>(sceneCamera());
	cameraEntity->getComponent<cameraComponent>().cam.setViewportSize(1280, 720);
	cameraEntity->addComponent<nativeScriptComponent>(nativeScriptComponent()).bind<cameraController>();
	activeScene->createEntity()->addComponent<spriteRenderComponent>();
	sp.setContext(activeScene);
}

void editor2d::onUpdate(float deltaTime)
{
	// resize
	//if (frameBufferProp prop = fb->prop;
	//	viewportSize.x > 0 && viewportSize.y > 0 &&
	//	(viewportSize.x != prop.width || viewportSize.y != prop.height))
	//{
	//	// resize的效果有点奇怪
	//	//fb->resize(viewportSize.x, viewportSize.y);
	//	activeScene->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
	//}

	// set up frame buffer
	//fb->bind();

	// test mouse position calculation
	//lighPos = glm::vec2(input::getmPos().first, input::getmPos().second);

	// prepare the renderer
	//renderCommand::setClear(glm::vec4(0, 0, 0.3f, 1));
	//glClearDepth(1.0f);
	//renderCommand::clear();

	// render the scene
	activeScene->onUpdate(deltaTime);

	// clean up
	//fb->unbind();
}

glm::vec2 editor2d::convertMousePos(int mod)
{
	float x = input::getmx();
	float y = input::getmy();
	uint32_t w = application::getApp().width;
	uint32_t h = application::getApp().height;
	if (mod == 0)
	{
		float yy = -1 * (y - (h / 2.0f)) * (cameraEntity->getComponent<cameraComponent>().cam.orthographicSize) / (h / 2.0f);
		float xx = (x - (w / 2.0f)) * (cameraEntity->getComponent<cameraComponent>().cam.orthographicSize
			* cameraEntity->getComponent<cameraComponent>().cam.aspectRatio) / (w / 2.0f);
		return glm::vec2(xx, yy) + glm::vec2(cameraEntity->getComponent<transformComponent>().translate[0],
			cameraEntity->getComponent<transformComponent>().translate[1]);
	}
	else
	{
		float yy = -1 * (y - (h / 2.0f)) / (h / 2.0f);
		float xx = (x - (w / 2.0f)) / (w / 2.0f);
		return glm::vec2(xx, yy) + glm::vec2(cameraEntity->getComponent<transformComponent>().translate[0],
			cameraEntity->getComponent<transformComponent>().translate[1]);
	}
}

void editor2d::onEvent(t_event& e)
{
	//t_dispatcher dispatcher(e);
	//dispatcher.dispatch<keyTypedEvent>(BIND_EVENT(editor2d::onKeyTyped));
	//dispatcher.dispatch<keyPressedEvent>(BIND_EVENT(editor2d::onKeyPressed));
}



void editor2d::guiRender()
{
	// scene interaction
	sp.onImguiRender();
}