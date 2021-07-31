#pragma once
#include "entt.h"
#include "glm/glm.hpp"
#include "graphic/camera.h"
#include "renderer/renderall.h"
#include "glm/ext/matrix_transform.hpp"

namespace tengine
{
	class scriptableEntity;

	struct transformComponent
	{
		glm::vec3 translate{ 0 };
		glm::vec3 rotation{ 0 };
		glm::vec3 scale{ 1 };
		uint32_t subjectTo = -1;

		transformComponent() = default;
		transformComponent(const transformComponent&) = default;

		void attach(uint32_t target)
		{
			subjectTo = target;
		}
		void dettach()
		{
			subjectTo = -1;
		}
		void moveForward(float dist)
		{
			glm::mat4 rotMatrix = 
				glm::rotate(glm::mat4(1), glm::radians(rotation[0]), glm::vec3(1, 0, 0))*
				glm::rotate(glm::mat4(1), glm::radians(rotation[1]), glm::vec3(0, 1, 0))*
				glm::rotate(glm::mat4(1), glm::radians(rotation[2]), glm::vec3(0, 0, 1));
			glm::vec4 mov = glm::vec4(dist, 0, 0, 1);
			mov = rotMatrix * mov;
			translate += glm::vec3(mov.x, mov.y, mov.z);
		}
		void moveUpward(float dist)
		{
			glm::mat4 rotMatrix =
				glm::rotate(glm::mat4(1), glm::radians(rotation[0]), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotation[1]), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotation[2]), glm::vec3(0, 0, 1));
			glm::vec4 mov = glm::vec4(0, dist, 0, 1);
			mov = rotMatrix * mov;
			translate += glm::vec3(mov.x, mov.y, mov.z);
		}
		void moveRight(float dist)
		{
			glm::mat4 rotMatrix =
				glm::rotate(glm::mat4(1), glm::radians(rotation[0]), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotation[1]), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotation[2]), glm::vec3(0, 0, 1));
			glm::vec4 mov = glm::vec4(0, 0, dist, 1);
			mov = rotMatrix * mov;
			translate += glm::vec3(mov.x, mov.y, mov.z);
		}
	};

	struct tagComponent
	{
		std::string tag = "default tag"; // maybe should be a enum class
		std::string name = "default name";
		uint32_t ID = -1;
		bool significant = true;
		tagComponent() = default;
		tagComponent(const tagComponent&) = default;
		tagComponent(const std::string& str1, const std::string& str2)
			: tag(str1), name(str2) {}
	};

	struct spriteRenderComponent
	{
		glm::vec4 color{ 1 };
		std::string texPath = DEFAULT_WHITE_PIC;

		spriteRenderComponent() = default;
		spriteRenderComponent(const spriteRenderComponent&) = default;
		spriteRenderComponent(const glm::vec4& vec) : color(vec) {}

		glm::vec2 coord1 = glm::vec2(0, 0);
		glm::vec2 coord2 = glm::vec2(1, 0);
		glm::vec2 coord3 = glm::vec2(0, 1);
		glm::vec2 coord4 = glm::vec2(1, 1);
		std::shared_ptr<texture2D> tex;
	};

	struct cameraComponent
	{
		sceneCamera cam;
		bool primary = true;
		bool fixRatio = false;
		cameraComponent() = default;
		cameraComponent(const cameraComponent&) = default;
		cameraComponent(const sceneCamera& c) : cam(c) {}
	};

	struct nativeScriptComponent
	{
		scriptableEntity* target;
		scriptableEntity*(*instantiate)();
		void(*kill)(nativeScriptComponent*);

		template <typename T>
		void bind()
		{
			instantiate = []() { return static_cast<scriptableEntity*>(new T()); };
			kill = [](nativeScriptComponent* comp) {delete comp->target; comp->target = 0; };
		}
	};

	struct meshComponent
	{
		struct pointInfo
		{
			glm::vec3 position; // 坐标点
			glm::vec2 texCoord; // 材质对应坐标
		};
		struct faceInfo
		{
			glm::vec4 color; // 颜色
			std::string texPath; // 材质
			std::shared_ptr<texture2D> texture; // 材质
			int p1, p2, p3; // 面
		};
		std::vector<pointInfo> mesh;
		std::vector<faceInfo> faces;

		meshComponent() = default;
		meshComponent(const meshComponent&) = default;
		
		void addPoint(const glm::vec3& pos = glm::vec3(0), const glm::vec2& cor = glm::vec2(0))
		{
			mesh.push_back(pointInfo{pos, cor});
		}
		void addFace(const glm::vec4& col = glm::vec4(1), const std::string& tex = DEFAULT_WHITE_PIC,
			int fa = 0, int fb = 0, int fc = 0)
		{
			if (fa >= mesh.size() ||
				fb >= mesh.size() ||
				fc >= mesh.size() ||
				fa < 0 || fb < 0 || fc < 0)
			{
				log("invalid face specified!", 2);
				return;
			}
			faces.push_back(faceInfo{col, tex, texture2D::create(tex.c_str()), fa, fb, fc});
		}
		void editPoint(int which,
			const glm::vec3& pos, const glm::vec2& cor)
		{
			pointInfo& item = mesh.at(which);
			item.position = pos;
			item.texCoord = cor;
		}
		void editFace(int which,
			const glm::vec4& col, const std::string& tex, 
			int fa, int fb, int fc)
		{
			if (fa >= mesh.size() ||
				fb >= mesh.size() ||
				fc >= mesh.size() ||
				fa < 0 || fb < 0 || fc < 0)
			{
				log("invalid face specified!", 2);
				return;
			}
			faceInfo& item = faces.at(which);
			item.color = col;
			item.texPath = tex;
			item.texture = texture2D::create(tex.c_str());
			item.p1 = fa;
			item.p2 = fb;
			item.p3 = fc;
		}
		void rmPoint(int which)
		{
			mesh.erase(std::next(mesh.begin(), which));
			int ind = 0;
			while (ind < faces.size())
			{
				faceInfo face = faces.at(ind);
				if (face.p1 > which)
					face.p1 -= 1;
				else if (face.p1 == which)
				{
					rmFace(ind);
					continue;
				}
				if (face.p2 > which)
					face.p2 -= 1;
				else if (face.p2 == which)
				{
					rmFace(ind);
					continue;
				}
				if (face.p3 > which)
					face.p3 -= 1;
				else if (face.p3 == which)
				{
					rmFace(ind);
					continue;
				}
				ind++;
			}
		}
		void rmFace(int which)
		{
			faces.erase(std::next(faces.begin(), which));
		}
		static bool faceValid(const faceInfo& target, int meshSize)
		{
			return
				target.p1 != target.p2 &&
				target.p1 != target.p3 &&
				target.p3 != target.p2 &&
				target.p1 >= 0 &&
				target.p2 >= 0 &&
				target.p3 >= 0 &&
				target.p1 < meshSize &&
				target.p2 < meshSize &&
				target.p3 < meshSize;
		}
	};

	struct lightComponent
	{
		float intensity = 1; // 强度
		glm::vec3 color = glm::vec3(1); // 颜色
		glm::vec3 direction = glm::vec3(0); // 方向，长度为0时光源变为点光源

		lightComponent() = default;
		lightComponent(const lightComponent& other) = default;
	};

	struct particleComponent2D
	{
		float lifetime = 0;
		glm::vec2 movSpeed = glm::vec2(0);
		float rotSpeed = 0;
		particleComponent2D() = default;
		particleComponent2D(const particleComponent2D&) = default;
	};
}