#include "tpch.h"
#include "camera.h"
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
			//log("invalid aspect ratio detected (0)", 2);
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
}