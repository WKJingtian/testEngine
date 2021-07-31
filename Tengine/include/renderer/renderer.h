#pragma once
#include "renderCommand.h"
#include "graphic/camera.h"
#include "shader/shaderall.h"
#include "texture.h"
#include "scene/component.h"

namespace tengine
{

	struct quadVertex
	{
		glm::vec3 pos;
		glm::vec2 cord;
		glm::vec4 color;
		float texID;
	};

	struct renderStorage
	{
		static const uint32_t maxQuad = 1024;
		static const uint32_t maxVertice = 1024 * 4;
		static const uint32_t maxIndex = 1024 * 6;
		static const uint32_t maxTex = 32;

		std::shared_ptr<vArray> va;
		std::shared_ptr<vBuffer> vb;
		std::unordered_map<std::string, std::shared_ptr<shader>> shaLib;

		uint32_t quadIndexCount = 0;
		quadVertex* qvBase = 0;
		quadVertex* qvPtr = 0;

		std::array<std::shared_ptr<texture2D>, maxTex> textureSlot;
		uint32_t textureSlotIndex = 1;

		float* lightInfo; // 10 per light: intensity(1), color(3), toward(3), position(3)
	};

	class renderer
	{
	public:
		static void onResized(uint32_t w, uint32_t h);
		static void beginScene(const orthographicCamera& cam);
		static void endScene();
		static void submit(const std::shared_ptr<vArray>& va, const std::shared_ptr<shader>& s,
			const orthographicCamera& c, const glm::mat4& transform = glm::mat4(1));
		static void init();

		inline static API getAPI() { return renderAPI::getAPI(); }
	};

	class renderer2D
	{
	public:
		static void beginScene(const camera& c, const glm::mat4& transform);
		static void endScene();
		static void init();
		static void shutdown();
		static void flush();

		static void setLight(const glm::mat4& lightTransform,
			const glm::vec3& lightColor, const glm::vec3& lightToward,
			float lightIntensity);
		static void drawQuad(
			const glm::mat4& transform,
			const glm::vec4& color,
			const std::string& texPath = DEFAULT_WHITE_PIC,
			const glm::vec2& coord1 = { 0,0 }, const glm::vec2& coord2 = { 1,0 },
			const glm::vec2& coord3 = { 0,1 }, const glm::vec2& coord4 = { 1,1 });
		static void drawQuad(
			const glm::mat4& transform,
			const glm::vec4& color,
			const std::shared_ptr<texture2D>& tex,
			const glm::vec2& coord1 = { 0,0 }, const glm::vec2& coord2 = { 1,0 },
			const glm::vec2& coord3 = { 0,1 }, const glm::vec2& coord4 = { 1,1 });
		static renderStorage storage2D;

	};

	class renderer3D
	{
	public:
		static void beginScene(const camera& c, const glm::mat4& transform);
		static void endScene();
		static void init();
		static void shutdown();
		static void flush();

		static void setLight(const glm::mat4& lightTransform,
			const glm::vec3& lightColor, const glm::vec3& lightToward,
			float lightIntensity);
		static void drawTri(const std::string& texPath, const glm::vec4& color,
			const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
			const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3
		);
		static void drawTri(const std::shared_ptr<texture2D>& tex, const glm::vec4& color,
			const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
			const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3
		);
		static void drawTri(const std::string& texPath,
			const glm::vec4& color1, const glm::vec4& color2, const glm::vec4& color3,
			const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
			const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3
		);
		static void drawTri(const std::shared_ptr<texture2D>& tex,
			const glm::vec4& color1, const glm::vec4& color2, const glm::vec4& color3,
			const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
			const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3
		);
		static renderStorage storage3D;
	};
}