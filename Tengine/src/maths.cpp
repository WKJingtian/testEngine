#include "tpch.h"
#include "math/mathall.h"

namespace tengine
{
	// vec2
	vec2::vec2(const float& x, const float& y)
	{ a = x; b = y; name = "vec2\0"; }
	vec2::vec2(const float& x, const float& y, const char* n)
	{ a = x; b = y; name = n; }
	vec2::vec2(const vec2& vec)
	{ a = vec.a; b = vec.b; name = vec.name; }
	vec2& vec2::add(const vec2& target)
	{
		this->a = this->a + target.a;
		this->b = this->b + target.b;
		return *this;
	}
	vec2& vec2::substract(const vec2& target)
	{
		this->a = this->a - target.a;
		this->b = this->b - target.b;
		return *this;
	}
	vec2& vec2::multiply(const vec2& target)
	{
		this->a = this->a * target.a;
		this->b = this->b * target.b;
		return *this;
	}
	vec2& vec2::divide(const vec2& target)
	{
		this->a = this->a / target.a;
		this->b = this->b / target.b;
		return *this;
	}
	bool vec2::isEqual(const vec2& target)
	{
		return a == target.a && b == target.b;
	}
	vec2 operator+(vec2 left, const vec2& right)
	{
		return left.add(right);
	}
	vec2 operator-(vec2 left, const vec2& right)
	{
		return left.substract(right);
	}
	vec2 operator*(vec2 left, const vec2& right)
	{
		return left.multiply(right);
	}
	vec2 operator/(vec2 left, const vec2& right)
	{
		return left.divide(right);
	}
	std::ostream& operator<< (std::ostream& ost, const vec2& target)
	{
		std::cout << target.name << "(" << target.a << " - " << target.b << ")\n";
		return ost;
	}

	// vec3
	vec3::vec3(const float& x, const float& y, const float& z)
	{
		a = x; b = y; c = z;
		name = "vec3\0";
	}
	vec3::vec3(const float& x, const float& y, const float& z, const char* n)
	{
		a = x; b = y; c = z;
		name = n;
	}
	vec3::vec3(const vec3& vec)
	{
		a = vec.a; b = vec.b; c = vec.c;
		name = vec.name;
	}
	vec3& vec3::add(const vec3& target)
	{
		a = a + target.a;
		b = b + target.b;
		c = c + target.c;
		return *this;
	}
	vec3& vec3::substract(const vec3& target)
	{
		a = a - target.a;
		b = b - target.b;
		c = c - target.c;
		return *this;
	}
	vec3& vec3::multiply(const vec3& target)
	{
		a = a * target.a;
		b = b * target.b;
		c = c * target.c;
		return *this;
	}
	vec3& vec3::divide(const vec3& target)
	{
		a = a / target.a;
		b = b / target.b;
		c = c / target.c;
		return *this;
	}
	bool vec3::isEqual(const vec3& target)
	{
		return a == target.a && b == target.b && c == target.c;
	}
	vec3 operator+(vec3 left, const vec3& right)
	{
		return left.add(right);
	}
	vec3 operator-(vec3 left, const vec3& right)
	{
		return left.substract(right);
	}
	vec3 operator*(vec3 left, const vec3& right)
	{
		return left.multiply(right);
	}
	vec3 operator/(vec3 left, const vec3& right)
	{
		return left.divide(right);
	}
	std::ostream& operator<< (std::ostream& ost, const vec3& target)
	{
		std::cout << target.name << "(" << target.a << " - " << target.b << " - " << target.c << ")\n";
		return ost;
	}

	// vec4
	vec4::vec4(const float& x, const float& y, const float& z, const float& w)
	{
		a = x; b = y; c = z; d = w;
		name = "vec4\0";
	}
	vec4::vec4(const float& x, const float& y, const float& z, const float& w, const char* n)
	{
		a = x; b = y; c = z; d = w;
		name = n;
	}
	vec4::vec4(const vec4& vec)
	{
		a = vec.a; b = vec.b; c = vec.c; d = vec.d;
		name = vec.name;
	}
	vec4& vec4::add(const vec4& target)
	{
		a = a + target.a;
		b = b + target.b;
		c = c + target.c;
		d = d + target.d;
		return *this;
	}
	vec4& vec4::substract(const vec4& target)
	{
		a = a - target.a;
		b = b - target.b;
		c = c - target.c;
		d = d - target.d;
		return *this;
	}
	vec4& vec4::multiply(const vec4& target)
	{
		a = a * target.a;
		b = b * target.b;
		c = c * target.c;
		d = d * target.d;
		return *this;
	}
	vec4& vec4::divide(const vec4& target)
	{
		a = a / target.a;
		b = b / target.b;
		c = c / target.c;
		d = d / target.d;
		return *this;
	}
	bool vec4::isEqual(const vec4& target)
	{
		return a == target.a && b == target.b && c == target.c && d == target.d;
	}
	vec4 operator+(vec4 left, const vec4& right)
	{
		return left.add(right);
	}
	vec4 operator-(vec4 left, const vec4& right)
	{
		return left.substract(right);
	}
	vec4 operator*(vec4 left, const vec4& right)
	{
		return left.multiply(right);
	}
	vec4 operator/(vec4 left, const vec4& right)
	{
		return left.divide(right);
	}
	std::ostream& operator<< (std::ostream& ost, const vec4& target)
	{
		std::cout << target.name << "(" << target.a << " - " << target.b << " - " << target.c << " - " << target.d << ")\n";
		return ost;
	}

	inline float toRadian(float degree)
	{
		return (float)degree * (float)M_PI / 180.0f;
	}

	mat4::mat4()
	{
		for (int i = 0; i < 16; i++) element[i] = 0;
	}
	mat4::mat4(float diag)
	{
		for (int i = 0; i < 16; i++)
		{
			element[i] = 0;
		}
		element[0] = diag;
		element[5] = diag;
		element[10] = diag;
		element[15] = diag;
	}
	mat4::mat4(float ele[16])
	{
		for (int i = 0; i < 16; i++)
		{
			element[i] = ele[i];
		}
	}
	mat4::mat4(const mat4& mat)
	{
		for (int i = 0; i < 16; i++) element[i] = mat.element[i];
	}
	mat4 mat4::identity()
	{
		return mat4(1.0f);
	}
	mat4& mat4::add(const mat4& target)
	{
		for (int i = 0; i < 16; i++) element[i] += target.element[i];
		return *this;
	}
	mat4& mat4::multiply(const mat4& target)
	{
		float result_element[16];
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				float sum = 0;
				for (int e = 0; e < 4; e++)
				{
					sum += element[y + e * 4] * target.element[e + x * 4];
				}
				result_element[y + x * 4] = sum;
			}
		}
		for (int i = 0; i < 16; i++) element[i] = result_element[i];
		return *this;
	}
	mat4& mat4::transpose(const mat4& target)
	{
		float result_element[16];
		result_element[0] = element[0];
		result_element[1] = element[4];
		result_element[2] = element[8];
		result_element[3] = element[12];
		result_element[4] = element[1];
		result_element[5] = element[5];
		result_element[6] = element[9];
		result_element[7] = element[13];
		result_element[8] = element[2];
		result_element[9] = element[6];
		result_element[10] = element[10];
		result_element[11] = element[14];
		result_element[12] = element[3];
		result_element[13] = element[7];
		result_element[14] = element[11];
		result_element[15] = element[15];
		for (int i = 0; i < 16; i++) element[i] = result_element[i];
		return *this;
	}

	mat4 mat4::getOrthographic(float right, float left, float top, float bottom, float n, float f)
	{
		mat4 result(1.0f);
		result.element[0] = 2.0f / (right - left);
		result.element[5] = 2.0f / (top - bottom);
		result.element[10] = 2.0f / (n - f);
		result.element[12] = (left + right) / (left - right);
		result.element[13] = (bottom + top) / (bottom - top);
		result.element[14] = (f + n) / (n - f);
		return result;
	}
	mat4 mat4::getPerspective(float fov, float aspect_ratio, float n, float f)
	{
		mat4 result(1.0f);
		result.element[0] = 1.0f / (aspect_ratio * tan(toRadian(fov / 2)));
		result.element[5] = 1.0f / tan(toRadian(fov / 2));
		result.element[10] = (n + f) / (n - f);
		result.element[11] = -1.0f;
		result.element[14] = (2.0f * n * f) / (n - f);
		result.element[15] = 0;
		return result;
	}
	mat4 mat4::getTransform(const vec3& translation)
	{
		mat4 result(1.0f);
		result.element[12] = translation.a;
		result.element[13] = translation.b;
		result.element[14] = translation.c;
		return result;
	}
	mat4 mat4::getRotation(float angle, const vec3& axis)
	{
		mat4 result(1.0f);
		float rad = toRadian(angle);
		float c = cos(rad);
		float s = sin(rad);
		float neg_c = 1.0f - c;
		float x = axis.a;
		float y = axis.b;
		float z = axis.c;

		result.element[0] = x * neg_c + c;
		result.element[1] = y * x * neg_c + y * s;
		result.element[2] = z * x * neg_c - z * s;
		result.element[4] = x * y * neg_c - z * s;
		result.element[5] = y * neg_c + c;
		result.element[6] = z * y * neg_c + x * s;
		result.element[8] = x * z * neg_c + y * s;
		result.element[9] = y * z * neg_c - x * s;
		result.element[10] = z * neg_c + c;

		return result;
	}
	mat4 mat4::getScale(const vec3& scale)
	{
		mat4 result(1.0f);
		result.element[0] = scale.a;
		result.element[5] = scale.b;
		result.element[10] = scale.c;
		return result;
	}
	std::ostream& operator << (std::ostream& ost, const mat4& mat)
	{
		std::cout << "4*4 matrix: " << std::endl;
		std::cout << mat.element[0] << ", " << mat.element[4] << ", " << mat.element[8] << ", " << mat.element[12] << std::endl;
		std::cout << mat.element[1] << ", " << mat.element[5] << ", " << mat.element[9] << ", " << mat.element[13] << std::endl;
		std::cout << mat.element[2] << ", " << mat.element[6] << ", " << mat.element[10] << ", " << mat.element[14] << std::endl;
		std::cout << mat.element[3] << ", " << mat.element[7] << ", " << mat.element[11] << ", " << mat.element[15] << std::endl;
		return ost;
	}
}