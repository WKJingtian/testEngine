#include "editor2d.h"
#include "imgui.h"
#include "tengine_core.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

editor2d::editor2d()
{

}

editor2d::~editor2d()
{
	//delete fb;
}

void editor2d::onAttach()
{
	// entity system
	activeScene = new scene();
	cameraEntity = activeScene->createEntity();
	cameraEntity->addComponent<cameraComponent>();
	cameraEntity->getComponent<cameraComponent>().setViewportSize(1280, 720);
	cameraEntity->addComponent<nativeScriptComponent>(nativeScriptComponent()).bind<cameraController>();
	activeScene->createEntity()->addComponent<sampleCube>();
	sp.setContext(activeScene);
}

void editor2d::onUpdate(float deltaTime)
{
	// render the scene
	activeScene->onUpdate(deltaTime);
}

void editor2d::onEvent(t_event& e)
{

}



void editor2d::guiRender()
{
	sp.onImguiRender();
}