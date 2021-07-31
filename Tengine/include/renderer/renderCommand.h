#pragma once
#include "renderAPI.h"

namespace tengine
{
	class renderCommand
	{
	public:
		inline static void resizeWin(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { renAPI->onResized(x, y, w, h); }
		inline static void init() { renAPI->init(); }
		inline static void clear() { renAPI->clear(); }
		inline static void setClear(const glm::vec4& c) { renAPI->setClear(c); }
		inline static void drawIndex(const std::shared_ptr<vArray>& va, uint32_t count = 0)
		{ renAPI->drawIndex(va, count); }
	private:
		static renderAPI* renAPI;
	};
}