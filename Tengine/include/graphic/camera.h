#pragma once
#include "glm/glm/glm.hpp"
#include "event/eventall.h"

namespace tengine
{
	class camera
	{
	public:
		camera() = default;
		camera(const glm::mat4& mat) : projection(mat) {}
		glm::mat4 projection = glm::mat4(1);
	};

	class sceneCamera : public camera
	{
	public:
		enum class camType { orthographic = 0, perspective = 1 };

		sceneCamera() { recalculate(); }
		sceneCamera(const glm::mat4& mat) : camera(mat) { recalculate(); }

		void setOrthographic(float size, float n, float f);
		void setPerspective(float FOV, float n, float f);
		void setViewportSize(uint32_t w, uint32_t h);
		void recalculate();

		camType type = camType::orthographic;
		float aspectRatio = 0.0f;

		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f;
		float orthographicFar = 1.0f;

		float perspectiveFOV = 45.0f;
		float perspectiveNear = 0.01f;
		float perspectiveFar = 1000.0f;
	};

	class spectiveCamera
	{
		glm::vec4 translation;
		glm::vec4 rotation;
		glm::vec2 focal;
		glm::vec2 c;
	};

	class orthographicCamera
	{
	public:
		orthographicCamera(float l, float r, float b, float t);
		void setPos(const glm::vec3& p) { pos = p; recalculate(); }
		void setRot(float r) { rot = r; recalculate(); }
		void setZoom(float r, float z);
		void setProject(float l, float r, float b, float t);

		glm::mat4 project;
		glm::mat4 view;
		glm::mat4 viewProject;
		glm::vec3 pos = glm::vec3(0,0,0);
		float rot = 0;

	private:
		void recalculate();
	};

	class cameraControl
	{
	public:
		cameraControl(float r, float z);
		void onUpdate(float deltaTime);
		void onEvent(t_event& e);
		orthographicCamera cam;

		float aspectRatio = 1;
		float zoomLevel = 1;
	private:
		bool onScrolled(mouseScrollEvent& e);
		bool onResized(windowResizeEvent& e);
		bool onPressed(keyPressedEvent& e);
		bool onTyped(keyTypedEvent& e);
		bool onButton(buttonPressedEvent& e);
		glm::vec2 forwardVec(float deltaTime);
		glm::vec2 rightVec(float deltaTime);

		float speed = 0.01f;
		float dtime = 0;
	};
}