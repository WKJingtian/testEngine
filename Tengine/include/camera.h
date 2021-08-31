#pragma once
#include "glm/glm.hpp"
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
}