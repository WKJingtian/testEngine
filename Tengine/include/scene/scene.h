#pragma once
#include "entt.h"
#include "glm/glm.hpp"
#include "component.h"

namespace tengine
{
	class entity;

	class scene
	{
		friend class entity;
		friend class scenePanel;
		friend class serializer;
	public:
		scene();
		~scene();
		void onUpdate(float deltatime);
		void resize(uint32_t x, uint32_t y);
		entity* createEntity();
		uint32_t IDconverter(uint32_t tagID);
		void destroyEntity(entity e);
		void destroyAll();
		bool isValid(entity e);
		entt::registry& getReg() { return reg; }
		void lightDebug();
	private:
		uint32_t viewportWidth = 0, viewportHeight = 0;
		entt::registry reg;
		uint32_t entity_id = 0;// gives unique id to each entity.
		bool environmentalLight = true;
	};
}