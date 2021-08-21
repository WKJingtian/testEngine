#pragma once
#include "tengine_core.h"
#include <iostream>

namespace tengine
{
	struct vec2
	{
		const char* name;
		float a, b;
		vec2(const float& x, const float& y);
		vec2(const float& x, const float& y, const char* n);
		vec2(const vec2& vec);
		vec2& add(const vec2& target);
		vec2& substract(const vec2& target);
		vec2& multiply(const vec2& target);
		vec2& divide(const vec2& target);
		bool isEqual(const vec2& target);
	};
	vec2 operator+(vec2 left, const vec2& right);
	vec2 operator-(vec2 left, const vec2& right);
	vec2 operator*(vec2 left, const vec2& right);
	vec2 operator/(vec2 left, const vec2& right);
	std::ostream& operator<< (std::ostream& ost, const vec2& target);

	struct vec3
	{
		const char* name;
		float a, b, c;
		vec3(const float& x, const float& y, const float& z);
		vec3(const float& x, const float& y, const float& z, const char* n);
		vec3(const vec3& vec);
		vec3& add(const vec3& target);
		vec3& substract(const vec3& target);
		vec3& multiply(const vec3& target);
		vec3& divide(const vec3& target);
		bool isEqual(const vec3& target);
	};
	vec3 operator+(vec3 left, const vec3& right);
	vec3 operator-(vec3 left, const vec3& right);
	vec3 operator*(vec3 left, const vec3& right);
	vec3 operator/(vec3 left, const vec3& right);
	std::ostream& operator<< (std::ostream& ost, const vec3& target);

	struct vec4
	{
		const char* name;
		float a, b, c, d;
		vec4(const float& x, const float& y, const float& z, const float& w);
		vec4(const float& x, const float& y, const float& z, const float& w, const char* n);
		vec4(const vec4& vec);
		vec4& add(const vec4& target);
		vec4& substract(const vec4& target);
		vec4& multiply(const vec4& target);
		vec4& divide(const vec4& target);
		bool isEqual(const vec4& target);
	};
	vec4 operator+(vec4 left, const vec4& right);
	vec4 operator-(vec4 left, const vec4& right);
	vec4 operator*(vec4 left, const vec4& right);
	vec4 operator/(vec4 left, const vec4& right);
	std::ostream& operator<< (std::ostream& ost, const vec4& target);
}