#pragma once
#include "../shader.h"

namespace tengine
{
	class openglShader : public shader
	{
	public:
		openglShader(const std::string& vsrc, const std::string& fsrc);
		openglShader(const std::string& src);
		~openglShader();

		void bind() const;
		void unbind() const;

		void setUnimat4(const std::string& n, const glm::mat4& val) const override;
		void setUnivec4(const std::string& n, const glm::vec4& val) const override;
		void setUnimat3(const std::string& n, const glm::mat3& val) const override;
		void setUnivec3(const std::string& n, const glm::vec3& val) const override;
		void setUnivec2(const std::string& n, const glm::vec2& val) const override;
		void setUniint(const std::string& n, int val) const override;
		void setUnifloat(const std::string& n, float val) const override;
		virtual void setUniintArray(const std::string& n, int* val, uint32_t count) const override;
		virtual void setUnifloatArray(const std::string& n, float* val, uint32_t count) const override;
	};
}