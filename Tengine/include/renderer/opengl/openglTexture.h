#pragma once

#include "../texture.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace tengine
{
	class openglTex2D : public texture2D
	{
	public:
		openglTex2D(const char* texPath);
		~openglTex2D();

		inline uint32_t getID() const { return renID; }
		virtual uint32_t getWidth() const override;
		virtual uint32_t getHeight() const override;
		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(uint32_t slot = 0) const override;
		virtual bool operator==(const texture& other) override
		{
			return src == other.src;
		}
	private:
		const char* path;
		uint32_t width, height;
		GLenum iFormat, dFormat;
	};
}