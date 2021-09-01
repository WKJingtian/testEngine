#pragma once
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include "imgui.h"
#include "bgfxDraw.h"

#include "tengine_all.h"
#include "scenePanel.h"

namespace tengine
{
	class editor2d : public t_layer
	{
	public:
		editor2d();
		~editor2d();
		void onAttach() override;
		void onUpdate(float deltaTime) override;
		void onEvent(t_event& e) override;
		void guiRender() override;

		// camera
		glm::vec3 transform = glm::vec3(1);
		glm::vec3 colorVec = glm::vec3(1);
		glm::vec2 lighPos = glm::vec3(0);
		glm::vec2 viewportSize;

		//// editor
		//frameBuffer* fb = 0;

		// scene
		ownedPtr<scene> activeScene = ownedPtr<scene>(nullptr);
		std::shared_ptr<entity> cameraEntity;
		scenePanel sp;
	};
}