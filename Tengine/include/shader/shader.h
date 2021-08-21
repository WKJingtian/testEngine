#pragma once
#include <string>
#include "glm/glm.hpp"

namespace tengine
{

	class shader
	{
	public:
		virtual ~shader() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static shader* create(const std::string& vsrc, const std::string& fsrc);
		static shader* create(const std::string& src);

		virtual void setUnimat4(const std::string& n, const glm::mat4& val) const = 0;
		virtual void setUnivec4(const std::string& n, const glm::vec4& val) const = 0;
		virtual void setUnimat3(const std::string& n, const glm::mat3& val) const = 0;
		virtual void setUnivec3(const std::string& n, const glm::vec3& val) const = 0;
		virtual void setUnivec2(const std::string& n, const glm::vec2& val) const = 0;
		virtual void setUniint(const std::string& n, int val) const = 0;
		virtual void setUnifloat(const std::string& n, float val) const = 0;
		virtual void setUniintArray(const std::string& n, int* val, uint32_t count) const = 0;
		virtual void setUnifloatArray(const std::string& n, float* val, uint32_t count) const = 0;
	protected:
		uint32_t renID;
	};
}