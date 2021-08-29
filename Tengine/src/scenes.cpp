#include "tpch.h"
#include "scene/sceneall.h"
#include "util/utilall.h"
#include "glm/ext/matrix_transform.hpp"
#include "yaml-cpp/yaml.h"
#include "imgui.h"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "bgfxDraw.h"
#include "application.h"

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			//std::cout << node.size() << "<- this is the size\n";
			//std::cout << node.IsSequence() << "if it is sequence\n";
			if (!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace tengine
{
	uint32_t colorConvert(const glm::vec4& color)
	{
		uint32_t result = ((uint8_t)(color.w * 255.0f) << 24)
			+ ((uint8_t)(color.z * 255.0f) << 16)
			+ ((uint8_t)(color.y * 255.0f) << 8)
			+ (uint8_t)(color.x * 255.0f);
		return result;
	}

	// scene
	scene::scene()
	{

	}
	scene::~scene()
	{

	}
	entity* scene::createEntity()
	{
		entity* result = new entity{ reg.create(), this };
		result->setComponent<transformComponent>(transformComponent());
		result->setComponent<tagComponent>(tagComponent()).ID = entity_id;
		entity_id++;
		return result;
	}
	void scene::destroyEntity(entity e)
	{
		if (e.hasComponent<nativeScriptComponent>())
			e.getComponent<nativeScriptComponent>().target->onDestroy();
		reg.destroy((entt::entity)e);
	}
	void scene::destroyAll()
	{
		reg.clear();
		entity_id = 0;
	}
	void scene::onUpdate(float deltatime)
	{
		auto view1 = reg.view<nativeScriptComponent>();
		{
			for (auto& e : view1)
			{
				auto& script = view1.get<nativeScriptComponent>(e);

				if (!script.target)
				{
					script.target = script.instantiate();
					script.target->ent = new entity(e, this);
					script.target->onCreate();
				}
				script.target->onUpdate(deltatime);
			}
		}
		auto view2 = reg.view<transformComponent, cameraComponent>();
		bool cameraFound = false;
		cameraComponent primaryCamera;
		entity cameraEntity;
		for (auto& e : view2)
		{
			auto& camera = view2.get<cameraComponent>(e);
			if (camera.primary)
			{
				cameraFound = true;
				primaryCamera = camera;
				cameraEntity = entity(e, this);
				break;
			}
		}
		if (!cameraFound) return;

		auto view3 = reg.view<lightComponent>();
		struct lightInfo
		{
			glm::vec3 toward;
			glm::vec3 position;
			glm::vec3 color;
			float intensity;
		};
		std::vector<lightInfo> lights;
		for (auto& e : view3)
		{
			entity temp = entity(e, this);
			lightComponent& comp = temp.getComponent<lightComponent>();
			lights.push_back(lightInfo{
				glm::length(comp.direction) == 0 ? glm::vec3(0) : glm::normalize(glm::vec3(
				glm::rotate(glm::mat4(1), temp.getComponent<transformComponent>().rotation.x, glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), temp.getComponent<transformComponent>().rotation.y, glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), temp.getComponent<transformComponent>().rotation.z, glm::vec3(0, 0, 1)) *
				glm::vec4(comp.direction, 1))),
				temp.getComponent<transformComponent>().translate,
				comp.color, comp.intensity
				});
		}

		const glm::mat4 inverted = glm::inverse(cameraEntity.transform());
		const glm::vec3 forward = glm::normalize(glm::vec3(inverted[2]));
		const glm::vec4 upward = cameraEntity.transform() * glm::vec4(0, 1, 0, 0);
		bgfx::setViewTransform(0,
			//glm::value_ptr(glm::inverse(cameraEntity.transform())),
			glm::value_ptr(glm::lookAt(
				cameraEntity.getComponent<transformComponent>().translate,
				forward,
				glm::normalize(glm::vec3(upward.x, upward.y, upward.z))
				)),
			glm::value_ptr(primaryCamera.getProjection())
			);

		PosColorVertex* vbPtr = &(editorApp::getApp()->vertexes[0]);
		uint32_t* ibPtr = &(editorApp::getApp()->indexes[0]);
		int counter = 0;

		auto view4 = reg.view<spriteRenderComponent>();
		for (auto e : view4)
		{
			//log("drawing sprite...", 0);
			entity temp = entity(e, this);
			spriteRenderComponent& comp = temp.getComponent<spriteRenderComponent>();
			bgfx::setTransform(&temp.transform());
			bgfx::setVertexBuffer(0, comp.getQVBO());
			bgfx::setIndexBuffer(comp.getQIBO());
			bgfx::submit(0, editorApp::getApp()->program);
		}

		auto view5 = reg.view<meshComponent>();
		for (auto e : view5)
		{
			entity temp = entity(e, this);
			meshComponent& comp = temp.getComponent<meshComponent>();
			for (auto& face : comp.faces)
			{
				if (!meshComponent::faceValid(face, (int)comp.mesh.size())) continue;
				//log("drawing face...", 0);
				//if (!face.texture || face.texture->src != face.texPath)
				//	face.texture = texture2D::create(face.texPath.c_str());
				meshComponent::pointInfo& vertex1 = comp.mesh.at(face.p1);
				meshComponent::pointInfo& vertex2 = comp.mesh.at(face.p2);
				meshComponent::pointInfo& vertex3 = comp.mesh.at(face.p3);
				glm::vec3 p1 = temp.transform() * glm::vec4(vertex1.position, 1);
				glm::vec3 p2 = temp.transform() * glm::vec4(vertex2.position, 1);
				glm::vec3 p3 = temp.transform() * glm::vec4(vertex3.position, 1);
		
				glm::vec3 lightOn1, lightOn2, lightOn3;
				if (environmentalLight)
				{
					lightOn1 = glm::vec3(1);
					lightOn2 = glm::vec3(1);
					lightOn3 = glm::vec3(1);
				}
				else
				{
					lightOn1 = glm::vec3(0);
					lightOn2 = glm::vec3(0);
					lightOn3 = glm::vec3(0);
				}
		
				for (lightInfo& l : lights)
				{
					glm::vec3 rPos1 = p1 - l.position;
					glm::vec3 rPos2 = p2 - l.position;
					glm::vec3 rPos3 = p3 - l.position;
					glm::vec3 d1(l.toward);
					glm::vec3 d2(l.toward);
					glm::vec3 d3(l.toward);
					glm::vec3 faceNormal = glm::normalize(glm::cross(p2 - p1, p2 - p3));
					if (glm::length(l.toward) == 0)
					{
						d1 = glm::normalize(rPos1);
						d2 = glm::normalize(rPos2);
						d3 = glm::normalize(rPos3);
					}
					if (glm::dot(d1, rPos1) > 0 &&
						glm::dot(d1, faceNormal) < 0)
					{
						float magnifier =
							-1 * glm::dot(d1, faceNormal) *
							(1.0f / sqrt(glm::length(rPos1))) * l.intensity;
						lightOn1 += glm::vec3(
							l.color.x / 255.0f * magnifier,
							l.color.y / 255.0f * magnifier,
							l.color.z / 255.0f * magnifier);
					}
					if (glm::dot(d2, rPos2) > 0 &&
						glm::dot(d2, faceNormal) < 0)
					{
						float magnifier =
							-1 * glm::dot(d2, faceNormal) *
							(1.0f / sqrt(glm::length(rPos2))) * l.intensity;
						lightOn2 += glm::vec3(
							l.color.x / 255.0f * magnifier,
							l.color.y / 255.0f * magnifier,
							l.color.z / 255.0f * magnifier);
					}
					if (glm::dot(d3, rPos3) > 0 &&
						glm::dot(d3, faceNormal) < 0)
					{
						float magnifier =
							-1 * glm::dot(d3, faceNormal) *
							(1.0f / sqrt(glm::length(rPos3))) * l.intensity;
						lightOn3 += glm::vec3(
							l.color.x / 255.0f * magnifier,
							l.color.y / 255.0f * magnifier,
							l.color.z / 255.0f * magnifier);
					}
				}
		
				glm::vec4 finalColor1 = glm::vec4(
					lightOn1.x * face.color.x,
					lightOn1.y * face.color.y,
					lightOn1.z * face.color.z,
					face.color.w);
				glm::vec4 finalColor2 = glm::vec4(
					lightOn2.x * face.color.x,
					lightOn2.y * face.color.y,
					lightOn2.z * face.color.z,
					face.color.w);
				glm::vec4 finalColor3 = glm::vec4(
					lightOn3.x * face.color.x,
					lightOn3.y * face.color.y,
					lightOn3.z * face.color.z,
					face.color.w);
				vbPtr[0] = { p1.x, p1.y, p1.z, colorConvert(finalColor1) };
				vbPtr[1] = { p2.x, p2.y, p2.z, colorConvert(finalColor2) };
				vbPtr[2] = { p3.x, p3.y, p3.z, colorConvert(finalColor3) };
				ibPtr[0] = 0 + counter; ibPtr[1] = 1 + counter; ibPtr[2] = 2 + counter;
				counter += 3;
				vbPtr += 3;
				ibPtr += 3;
			}
		}

		auto view6 = reg.view<uiShape>();
		for (auto e : view6)
		{
			//log("drawing sprite...", 0);
			entity temp = entity(e, this);
			uiShape& comp = temp.getComponent<uiShape>();
			//PosColorVertex vbPtr[3];
			//uint32_t ibPtr[3];
			glm::vec4 p1 = temp.transform() * glm::vec4(comp.p1, 1);
			glm::vec4 p2 = temp.transform() * glm::vec4(comp.p2, 1);
			glm::vec4 p3 = temp.transform() * glm::vec4(comp.p3, 1);
			vbPtr[0] = { p1.x, p1.y, p1.z, 0xFFFFFFFF };
			vbPtr[1] = { p2.x, p2.y, p2.z, 0xFFFFFFFF };
			vbPtr[2] = { p3.x, p3.y, p3.z, 0xFFFFFFFF };
			ibPtr[0] = 0 + counter; ibPtr[1] = 1 + counter; ibPtr[2] = 2 + counter;
			counter += 3;
			vbPtr += 3;
			ibPtr += 3;
		}

		auto view7 = reg.view<sampleCube>();
		for (auto e : view7)
		{
			entity temp = entity(e, this);
			sampleCube& comp = temp.getComponent<sampleCube>();


			static PosColorVertex CVBO[] =
			{
				{-1.0f,  1.0f,  1.0f, 0xff000000 },
				{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
				{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
				{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
				{-1.0f,  1.0f, -1.0f, 0xffff0000 },
				{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
				{-1.0f, -1.0f, -1.0f, 0xffffff00 },
				{ 1.0f, -1.0f, -1.0f, 0xffffffff },
			};
			static const uint16_t CIBO[] = { 2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4, 3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6 };

			static bgfx::VertexLayout layout = bgfx::VertexLayout();
			layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.end();
			static bgfx::VertexBufferHandle cvboHandle =
				bgfx::createVertexBuffer(bgfx::makeRef(CVBO, sizeof(CVBO)), layout);
			static bgfx::IndexBufferHandle ciboHandle =
				bgfx::createIndexBuffer(bgfx::makeRef(CIBO, sizeof(CIBO)));

			bgfx::setTransform(&temp.transform());
			bgfx::setVertexBuffer(0, cvboHandle);
			bgfx::setIndexBuffer(ciboHandle);
			bgfx::submit(0, editorApp::getApp()->program);
		}
	}

	void scene::lightDebug()
	{
		ImGui::Begin("light debug panel");
		auto view3 = reg.view<lightComponent>();
		struct lightInfo
		{
			glm::vec3 toward;
			glm::vec3 position;
			glm::vec3 color;
			float intensity;
		};
		std::vector<lightInfo> lights;
		for (auto& e : view3)
		{
			entity temp = entity(e, this);
			lightComponent& comp = temp.getComponent<lightComponent>();
			lights.push_back(lightInfo{
				glm::length(comp.direction) == 0 ? glm::vec3(0) : glm::normalize(glm::vec3(
				glm::rotate(glm::mat4(1), temp.getComponent<transformComponent>().rotation.x, glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), temp.getComponent<transformComponent>().rotation.y, glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), temp.getComponent<transformComponent>().rotation.z, glm::vec3(0, 0, 1)) *
				glm::vec4(comp.direction, 1))),
				temp.getComponent<transformComponent>().translate,
				comp.color, comp.intensity
				});
		}

		auto view5 = reg.view<meshComponent>();
		int meshCount = 0;
		for (auto e : view5)
		{
			entity temp = entity(e, this);
			meshComponent& comp = temp.getComponent<meshComponent>();
			int faceCount = 0;
			for (auto& face : comp.faces)
			{
				if (!meshComponent::faceValid(face, (int)comp.mesh.size())) continue;
				//if (!face.texture || face.texture->src != face.texPath)
				//	face.texture = texture2D::create(face.texPath.c_str());
				meshComponent::pointInfo& vertex1 = comp.mesh.at(face.p1);
				meshComponent::pointInfo& vertex2 = comp.mesh.at(face.p2);
				meshComponent::pointInfo& vertex3 = comp.mesh.at(face.p3);
				glm::vec3 p1 = temp.transform() * glm::vec4(vertex1.position, 1);
				glm::vec3 p2 = temp.transform() * glm::vec4(vertex2.position, 1);
				glm::vec3 p3 = temp.transform() * glm::vec4(vertex3.position, 1);

				glm::vec3 lightOn1, lightOn2, lightOn3;
				if (environmentalLight)
				{
					lightOn1 = glm::vec3(1);
					lightOn2 = glm::vec3(1);
					lightOn3 = glm::vec3(1);
				}
				else
				{
					lightOn1 = glm::vec3(0);
					lightOn2 = glm::vec3(0);
					lightOn3 = glm::vec3(0);
				}
				int lightCount = 0;
				for (lightInfo& l : lights)
				{
					glm::vec3 rPos1 = p1 - l.position;
					glm::vec3 rPos2 = p2 - l.position;
					glm::vec3 rPos3 = p3 - l.position;
					glm::vec3 d1(l.toward);
					glm::vec3 d2(l.toward);
					glm::vec3 d3(l.toward);
					glm::vec3 faceNormal = glm::normalize(glm::cross(p2 - p1, p2 - p3));
					if (glm::length(l.toward) == 0)
					{
						d1 = glm::normalize(rPos1);
						d2 = glm::normalize(rPos2);
						d3 = glm::normalize(rPos3);
					}
					ostringstream ss;
					ss << "mesh " << meshCount << " face " << faceCount << " light " << lightCount;
					if (ImGui::TreeNodeEx(ss.str().c_str()))
					{
						ImGui::Text("p1 position: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(rPos1).c_str());
						ImGui::Text("p2 position: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(rPos2).c_str());
						ImGui::Text("p3 position: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(rPos3).c_str());
						ImGui::Text("d1: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(d1).c_str());
						ImGui::Text("d2: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(d2).c_str());
						ImGui::Text("d3: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(d3).c_str());
						ImGui::Text("normal: ");
						ImGui::SameLine();
						ImGui::Text(glm::to_string(faceNormal).c_str());
						ImGui::TreePop();
					}
					lightCount++;
				}
				faceCount++;
			}
			meshCount++;
		}
		ImGui::End();
	}
	void scene::resize(uint32_t x, uint32_t y)
	{
		viewportWidth = x;
		viewportHeight = y;
		auto view = reg.view<cameraComponent>();
		for (auto& e : view)
		{
			auto& cc = view.get<cameraComponent>(e);
			if (!cc.fixRatio) cc.setViewportSize(x, y);
		}
	}
	bool scene::isValid(entity e)
	{
		return reg.valid(e.ent);
	}
	uint32_t scene::IDconverter(uint32_t tagID)
	{
		auto view = reg.view<tagComponent>();
		for (entt::entity e : view)
		{
			tagComponent comp = view.get<tagComponent>(e);
			if (comp.ID == tagID) return (uint32_t)e;
		}
		return (uint32_t)entt::null;
	}

	// entity
	entity::entity(entt::entity e, scene* s)
		: ent(e), sce(s)
	{

	}

	// serializer
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec2& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

	static void serializeEntity(YAML::Emitter& out, entity ent)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "entity" << YAML::Value << (uint32_t)ent;

		if (ent.hasComponent<tagComponent>())
		{
			auto& comp = ent.getComponent<tagComponent>();
			out << YAML::Key << "tag component";
			out << YAML::BeginMap;
			out << YAML::Key << "tag" << YAML::Value << comp.tag;
			out << YAML::Key << "name" << YAML::Value << comp.name;
			out << YAML::Key << "ID" << YAML::Value << comp.ID;
			out << YAML::Key << "significance" << YAML::Value << comp.significant;
			out << YAML::EndMap;
		}
		if (ent.hasComponent<transformComponent>())
		{
			auto& comp = ent.getComponent<transformComponent>();
			out << YAML::Key << "transform component";
			out << YAML::BeginMap;
			out << YAML::Key << "translate" << YAML::Value << comp.translate;
			out << YAML::Key << "scale" << YAML::Value << comp.scale;
			out << YAML::Key << "rotation" << YAML::Value << comp.rotation;
			out << YAML::Key << "subject to" << YAML::Value << comp.subjectTo;
			out << YAML::EndMap;
		}
		if (ent.hasComponent<cameraComponent>())
		{
			auto& comp = ent.getComponent<cameraComponent>();
			out << YAML::Key << "camera component";
			out << YAML::BeginMap;
			//out << YAML::Key << "camera type" << YAML::Value << (int)comp.type;
			out << YAML::Key << "perspective FOV" << YAML::Value << comp.perspectiveFOV;
			out << YAML::Key << "perspective near" << YAML::Value << comp.perspectiveNear;
			out << YAML::Key << "perspective far" << YAML::Value << comp.perspectiveFar;
			out << YAML::Key << "orthographic size" << YAML::Value << comp.orthographicSize;
			out << YAML::Key << "orthographic near" << YAML::Value << comp.orthographicNear;
			out << YAML::Key << "orthographic far" << YAML::Value << comp.orthographicFar;
			out << YAML::Key << "aspect ratio" << YAML::Value << comp.aspectRatio;

			out << YAML::Key << "is primary" << YAML::Value << comp.primary;
			out << YAML::Key << "is fixed ratio" << YAML::Value << comp.fixRatio;
			out << YAML::EndMap;
		}
		if (ent.hasComponent<spriteRenderComponent>())
		{
			auto& comp = ent.getComponent<spriteRenderComponent>();
			out << YAML::Key << "sprite component";
			out << YAML::BeginMap;
			out << YAML::Key << "color" << YAML::Value << comp.color;
			out << YAML::Key << "texture path" << YAML::Value << comp.texPath;
			out << YAML::Key << "coord1" << YAML::Value << comp.coord1;
			out << YAML::Key << "coord2" << YAML::Value << comp.coord2;
			out << YAML::Key << "coord3" << YAML::Value << comp.coord3;
			out << YAML::Key << "coord4" << YAML::Value << comp.coord4;
			out << YAML::EndMap;
		}
		if (ent.hasComponent<meshComponent>())
		{
			auto& comp = ent.getComponent<meshComponent>();
			out << YAML::Key << "mesh component";
			out << YAML::BeginMap;
			out << YAML::Key << "points" << YAML::Value << YAML::BeginSeq;
			for (auto& item : comp.mesh)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "position" << YAML::Value << item.position;
				out << YAML::Key << "texcoord" << YAML::Value << item.texCoord;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::Key << "faces" << YAML::Value << YAML::BeginSeq;
			for (auto& item : comp.faces)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "color" << YAML::Value << item.color;
				out << YAML::Key << "texture" << YAML::Value << item.texPath;
				out << YAML::Key << "p1" << YAML::Value << item.p1;
				out << YAML::Key << "p2" << YAML::Value << item.p2;
				out << YAML::Key << "p3" << YAML::Value << item.p3;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}
		if (ent.hasComponent<lightComponent>())
		{
			auto& comp = ent.getComponent<lightComponent>();
			out << YAML::Key << "light component";
			out << YAML::BeginMap;
			out << YAML::Key << "intensity" << YAML::Value << comp.intensity;
			out << YAML::Key << "color" << YAML::Value << comp.color;
			out << YAML::Key << "direction" << YAML::Value << comp.direction;
			out << YAML::EndMap;
		}
		if (ent.hasComponent<particleComponent2D>())
		{
			auto& comp = ent.getComponent<particleComponent2D>();
			out << YAML::Key << "particle component";
			out << YAML::BeginMap;
			out << YAML::Key << "lifetime" << YAML::Value << comp.lifetime;
			out << YAML::Key << "move speed" << YAML::Value << comp.movSpeed;
			out << YAML::Key << "rotation speed" << YAML::Value << comp.rotSpeed;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	serializer::serializer(scene* sce)
		: target(sce)
	{

	}
	void serializer::serialize(const std::string path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "scene" << YAML::Value << "untitled";
		out << YAML::Key << "entity" << YAML::Value << YAML::BeginSeq;
		target->reg.each([&](entt::entity entID) {
			entity ent = entity(entID, target);
			if (!ent) return;
			serializeEntity(out, ent);
			});
		out << YAML::EndSeq;
		out << YAML::Key << "entity count" << YAML::Value << target->entity_id;
		out << YAML::Key << "environment light" << YAML::Value << target->environmentalLight;
		out << YAML::EndMap;
		std::ofstream fout(path);
		fout << out.c_str();
	}
	void serializer::deserialize(const std::string path)
	{
		std::stringstream src;
		src << std::ifstream(path).rdbuf();
		YAML::Node data = YAML::Load(src.str());
		if (!data["scene"] || !data["entity count"])
		{
			log("save does not exist, loading failed", 3);
			return;
		}
		log("Now deserialize scene " + data["scene"].as<std::string>(), 1);
		target->destroyAll();

		if (auto ents = data["entity"])
		{
			for (auto ent : ents)
			{
				if (!ent["transform component"] || !ent["tag component"])
				{
					log("invalid entity detected, skip...", 2);
					continue;
				}
				entity* newEnt = target->createEntity();
				newEnt->getComponent<tagComponent>().tag = ent["tag component"]["tag"].as<std::string>();
				newEnt->getComponent<tagComponent>().name = ent["tag component"]["name"].as<std::string>();
				newEnt->getComponent<tagComponent>().ID = ent["tag component"]["ID"].as<uint32_t>();
				newEnt->getComponent<tagComponent>().significant = ent["tag component"]["significance"].as<bool>();
				newEnt->getComponent<transformComponent>().translate = ent["transform component"]["translate"].as<glm::vec3>();
				newEnt->getComponent<transformComponent>().scale = ent["transform component"]["scale"].as<glm::vec3>();
				newEnt->getComponent<transformComponent>().rotation = ent["transform component"]["rotation"].as<glm::vec3>();
				newEnt->getComponent<transformComponent>().subjectTo = ent["transform component"]["subject to"].as<uint32_t>();
				if (ent["camera component"])
				{
					auto& comp = newEnt->addComponent<cameraComponent>();
					//comp.cam.type = (sceneCamera::camType)ent["camera component"]["camera type"].as<int>();
					comp.fixRatio = ent["camera component"]["is fixed ratio"].as<bool>();
					comp.primary = ent["camera component"]["is primary"].as<bool>();
					comp.aspectRatio = ent["camera component"]["aspect ratio"].as<float>();
					comp.perspectiveFOV = ent["camera component"]["perspective FOV"].as<float>();
					comp.perspectiveNear = ent["camera component"]["perspective near"].as<float>();
					comp.perspectiveFar = ent["camera component"]["perspective far"].as<float>();
					comp.orthographicSize = ent["camera component"]["orthographic size"].as<float>();
					comp.orthographicNear = ent["camera component"]["orthographic near"].as<float>();
					comp.orthographicFar = ent["camera component"]["orthographic far"].as<float>();
				}
				if (ent["sprite component"])
				{
					auto& comp = newEnt->addComponent<spriteRenderComponent>();
					comp.color = ent["sprite component"]["color"].as<glm::vec4>();
					comp.texPath = ent["sprite component"]["texture path"].as<std::string>();
					comp.coord1 = ent["sprite component"]["coord1"].as<glm::vec2>();
					comp.coord2 = ent["sprite component"]["coord2"].as<glm::vec2>();
					comp.coord3 = ent["sprite component"]["coord3"].as<glm::vec2>();
					comp.coord4 = ent["sprite component"]["coord4"].as<glm::vec2>();
				}
				if (ent["mesh component"])
				{
					auto& comp = newEnt->addComponent<meshComponent>();
					for (auto p : ent["mesh component"]["points"])
						comp.addPoint(p["position"].as<glm::vec3>(), p["texcoord"].as<glm::vec2>());
					for (auto f : ent["mesh component"]["faces"])
						comp.addFace(f["color"].as<glm::vec4>(), f["texture"].as<std::string>(),
							f["p1"].as<int>(), f["p2"].as<int>(), f["p3"].as<int>());
				}
				if (ent["light component"])
				{
					auto& comp = newEnt->addComponent<lightComponent>();
					comp.intensity = ent["light component"]["intensity"].as<float>();
					comp.color = ent["light component"]["color"].as<glm::vec3>();
					comp.direction = ent["light component"]["direction"].as<glm::vec3>();
				}
				if (ent["particle component"])
				{
					auto& comp = newEnt->addComponent<particleComponent2D>();
					comp.lifetime = ent["particle component"]["lifetime"].as<float>();
					comp.movSpeed = ent["particle component"]["move speed"].as<glm::vec3>();
					comp.rotSpeed = ent["particle component"]["rotation speed"].as<float>();
				}
			}
		}
		target->entity_id = data["entity count"].as<uint32_t>();
		target->environmentalLight = data["environment light"].as<bool>();
	}

	void serializer::renameSave(const char* newPath, const char* oldPath)
	{
		rename(oldPath, newPath);
	}
}