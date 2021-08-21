#pragma once
#include "tengine_core.h"
#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include "vector.h"

namespace tengine
{
	struct mat4
	{
		float element[16]; // 4*4
		mat4();
		mat4(float diag);
		mat4(const mat4& mat);
		mat4(float ele[16]);
		static mat4 identity();
		mat4& add(const mat4& target);
		mat4& multiply(const mat4& target);
		mat4& transpose(const mat4& target);

		static mat4 getOrthographic(float right, float left, float top, float bottom, float near, float far);
		static mat4 getPerspective(float fov, float aspect_ratio, float near, float far);
		static mat4 getTransform(const vec3& translation);
		static mat4 getRotation(float angle, const vec3& axis);
		static mat4 getScale(const vec3& scale);
	};

	std::ostream& operator << (std::ostream& ost, const mat4& mat);
}