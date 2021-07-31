#pragma once
#include "../renderAPI.h"

namespace tengine
{
	class openglRenderAPI : public renderAPI
	{
	public:
		virtual void init() override;
		virtual void setClear(const glm::vec4& c) override;
		virtual void clear() override;
		virtual void drawIndex(const std::shared_ptr<vArray>& va, uint32_t count) override;
		virtual void onResized(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
	};
}