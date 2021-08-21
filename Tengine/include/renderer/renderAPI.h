#pragma once
#include "glm/glm.hpp"
#include "vertexArray.h"

namespace tengine
{
	enum class API
	{
		None = 0,
		opengl = 1,
		directx11 = 2,
		directx12 = 3,
		vulkan = 4
	};

	class renderAPI
	{
	public:
		virtual void init() = 0;
		virtual void clear() = 0;
		virtual void setClear(const glm::vec4& c) = 0;
		virtual void drawIndex(const std::shared_ptr<vArray>& va, uint32_t count = 0) = 0;
		virtual void onResized(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
		inline static API getAPI() { return api; }
		static API api;
	};
}