#pragma once
#include "scene.h"
#include "util/utilall.h"
#include "input.h"

namespace tengine
{
	class entity
	{
		friend class scene;
	public:
		bool significant = true;

		entity() = default;
		entity(entt::entity e, scene* s);
		entity(const entity&) = default;
		~entity() = default;

		scene* getScene() { return sce; }

		template <typename T>
		bool hasComponent()
		{
			return sce->getReg().all_of<T>(ent);
		}
		template <typename T>
		T& getComponent()
		{
			t_assert(hasComponent<T>(), 3, "Entity does not have the component you want!");
			return sce->getReg().get<T>(ent);
		}
		template <typename T>
		T& addComponent(T data)
		{
			t_assert(!hasComponent<T>(), 3, "Entity already has the component you want!");
			return sce->getReg().emplace<T>(ent, data);
		}
		template <typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			t_assert(!hasComponent<T>(), 3, "Entity already has the component you want!");
			return sce->getReg().emplace<T>(ent, std::forward<Args>(args)...);
		}
		template <typename T>
		void removeComponent()
		{
			t_assert(hasComponent<T>(), 3, "Entity does not have the component you want!");
			sce->getReg().remove<T>(ent);
		}
		template <typename T>
		T& setComponent(T data)
		{
			return sce->getReg().emplace_or_replace<T>(ent, data);
		}
		template <typename T, typename... Args>
		T& setComponent(Args&&... args)
		{
			return sce->getReg().emplace_or_replace<T>(ent, std::forward<Args>(args)...);
		}

		glm::mat4 transform()
		{
			transformComponent comp = getComponent<transformComponent>();
			entity e = entity((entt::entity)(sce->IDconverter(comp.subjectTo)), sce);

			if (!e || e==*this)
				return glm::translate(glm::mat4(1), comp.translate) *
				glm::scale(glm::mat4(1), comp.scale) *
				glm::rotate(glm::mat4(1), glm::radians(comp.rotation[0]), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(comp.rotation[1]), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(comp.rotation[2]), glm::vec3(0, 0, 1));
			else
			{
				return e.transform() *
					glm::translate(glm::mat4(1), comp.translate) *
					glm::scale(glm::mat4(1), comp.scale) *
					glm::rotate(glm::mat4(1), glm::radians(comp.rotation[0]), glm::vec3(1, 0, 0)) *
					glm::rotate(glm::mat4(1), glm::radians(comp.rotation[1]), glm::vec3(0, 1, 0)) *
					glm::rotate(glm::mat4(1), glm::radians(comp.rotation[2]), glm::vec3(0, 0, 1));
			}
		}

		operator bool() const { return sce != 0 && sce->isValid(*this); }
		operator entt::entity() const { return ent; }
		operator uint32_t() const { return (uint32_t)ent; }
		bool operator ==(const entity& other) const
		{
			return (uint32_t)ent == (uint32_t)other.ent && sce == other.sce;
		}
	private:
		friend scriptableEntity;
		entt::entity ent = entt::null;
		scene* sce = 0;
	};

	class scriptableEntity
	{
		friend class scene;
	public:
		entity* ent = 0;

		virtual int getID() { return -1; }

		scriptableEntity() = default;
		virtual ~scriptableEntity() = default;

		virtual void onCreate() {}
		virtual void onUpdate(float deltatime) {}
		virtual void onDestroy() {}
	};

	class cameraController : public scriptableEntity
	{
	public:
		virtual int getID() override { return 0; }
		float speed = 3.0f;

		cameraController() = default;
		~cameraController() = default;
		virtual void onCreate() override
		{

		}
		virtual void onUpdate(float deltatime) override
		{
			auto& tra = ent->getComponent<transformComponent>();
			if (input::isKeyPressed(T_KEY_LEFT_SHIFT))
			{
				// x
				if (input::isKeyPressed(T_KEY_A))
					tra.moveForward(-speed * NANO_TO_SEC);
				if (input::isKeyPressed(T_KEY_D))
					tra.moveForward(speed * NANO_TO_SEC);
				// y
				if (input::isKeyPressed(T_KEY_SPACE) && input::isKeyPressed(T_KEY_LEFT_CONTROL))
					tra.moveUpward(-speed * NANO_TO_SEC);
				else if (input::isKeyPressed(T_KEY_SPACE))
					tra.moveUpward(speed * NANO_TO_SEC);
				// z
				if (input::isKeyPressed(T_KEY_W))
					tra.moveRight(speed * NANO_TO_SEC);
				if (input::isKeyPressed(T_KEY_S))
					tra.moveRight(-speed * NANO_TO_SEC);
			}
			else
			{
				// x
				if (input::isKeyPressed(T_KEY_A))
					tra.translate[0] -= speed * NANO_TO_SEC;
				if (input::isKeyPressed(T_KEY_D))
					tra.translate[0] += speed * NANO_TO_SEC;
				// y
				if (input::isKeyPressed(T_KEY_SPACE) && input::isKeyPressed(T_KEY_LEFT_CONTROL))
					tra.translate[1] -= speed * NANO_TO_SEC;
				else if (input::isKeyPressed(T_KEY_SPACE))
					tra.translate[1] += speed * NANO_TO_SEC;
				// z
				if (input::isKeyPressed(T_KEY_W))
					tra.translate[2] += speed * NANO_TO_SEC;
				if (input::isKeyPressed(T_KEY_S))
					tra.translate[2] -= speed * NANO_TO_SEC;
			}
		}
		virtual void onDestroy() override
		{
			
		}
	};

	class particleSystem2D : public scriptableEntity
	{
		inline float getRand()
		{
			return ((float)(rand() % 2001 - 1000) / 1000.0f);
		}
	public:
		virtual int getID() override { return 1; }
		glm::vec2 velocity = glm::vec2(0.5f);
		glm::vec2 velocityVariation = glm::vec2(1);
		glm::vec4 colorBegin = glm::vec4(1), colorEnd = glm::vec4(0);
		glm::vec2 sizeBegin = glm::vec4(1), sizeEnd = glm::vec4(0.01f);
		float lifetime = 30;
		float rotSpeed = 0;
		char* texPath = DEFAULT_WHITE_PIC;
		int maxParticle = 128;
		int maxGenerationPerFrame = 1;
		int particleCount = 0;
		std::vector<entity*>particleList = std::vector<entity*>();
		bool emitting = true;

		particleSystem2D() = default;
		~particleSystem2D() = default;
		virtual void onCreate() override
		{

		}
		virtual void onUpdate(float deltatime) override
		{
			int alreadyMade = 0;
			vector<int> temp;
			int cur = 0;
			for (auto p : particleList)
			{
				particleComponent2D& particle = p->getComponent<particleComponent2D>();
				transformComponent& transform = p->getComponent<transformComponent>();
				spriteRenderComponent& renderSprite = p->getComponent<spriteRenderComponent>();
				particle.lifetime -= NANO_TO_SEC;
				if (particle.lifetime > 0)
				{
					transform.translate += glm::vec3(particle.movSpeed, 0) * NANO_TO_SEC;
					transform.rotation += glm::vec3(0, 0, particle.rotSpeed) * NANO_TO_SEC;
					transform.scale = glm::vec3(sizeBegin + ((particle.lifetime / lifetime) * (sizeBegin - sizeEnd)), 1);
					renderSprite.color = colorEnd + ((particle.lifetime / lifetime) * (colorBegin - colorEnd));
					cur++;
				}
				else
				{
					temp.push_back(cur);
				}
			}
			for (auto p : temp)
			{
				ent->getScene()->destroyEntity(**(particleList.begin() + p));
				particleList.erase(particleList.begin() + p);
				particleCount--;
			}
			while (emitting && particleCount < maxParticle && alreadyMade < maxGenerationPerFrame)
			{
				entity* e = ent->getScene()->createEntity();
				spriteRenderComponent& c1 = e->addComponent<spriteRenderComponent>();
				particleComponent2D& c2 = e->addComponent<particleComponent2D>();
				transformComponent& c3 = e->getComponent<transformComponent>();
				tagComponent& c4 = e->getComponent<tagComponent>();
				
				c1.color = colorBegin;
				c1.texPath = texPath;
				
				c2.lifetime = lifetime;
				c2.movSpeed = glm::vec2(velocity.x + (velocityVariation.x * getRand()),
					velocity.y + (velocityVariation.y * getRand()));
				c2.rotSpeed = rotSpeed * getRand();
				
				c3.translate = ent->getComponent<transformComponent>().translate;
				c3.rotation = glm::vec3(0);
				c3.scale = glm::vec3(sizeBegin, 1);

				c4.name = "particle generated by" + this->ent->getComponent<tagComponent>().name;
				c4.tag = "particle";
				c4.significant = false;

				e->significant = false;

				particleList.push_back(e);

				particleCount++;
				alreadyMade++;
			}
		}
		virtual void onDestroy() override
		{
			log("particle system on destroy", 2);
			for (auto p : particleList)
			{
				ent->getScene()->destroyEntity(*p);
			}
		}
	};
}