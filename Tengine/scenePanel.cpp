#include "tpch.h"
#include "scenePanel.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui_internal.h"

namespace tengine
{
	static void triValuePanel(const std::string& name, glm::vec3& value)
	{
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 120.0f);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ImGui::Text("X");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
		ImGui::Text("Y");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 1, 1));
		ImGui::Text("Z");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z);
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	// scene panel
	scenePanel::scenePanel()
		: selectedEntity(entity())
	{
		setContext(0);
	}
	scenePanel::scenePanel(scene* sce)
		: selectedEntity(entity())
	{
		setContext(sce);
	}
	void scenePanel::setContext(scene* sce)
	{
		context = sce;
	}
	void scenePanel::onImguiRender()
	{
		ImGui::Begin("Control Panel");
		if (ImGui::Button("Create Entity", ImVec2(100.0f, 30.0f)))
			context->createEntity();
		ImGui::SameLine();
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Close"))
				application::getApp().onEvent(windowCloseEvent());
			if (ImGui::MenuItem("new scene"))
				context->destroyAll();
			if (ImGui::MenuItem("save scene") && !saving)
			{
				saving = true;
				memset(savepath, 0, 1024);
			}
			if (ImGui::MenuItem("load scene") && !loading)
			{
				loading = true;
				memset(loadpath, 0, 1024);
			}
			ImGui::EndMenu();
		}
		ImGui::Checkbox("environmental light", &(context->environmentalLight));
		ImGui::Checkbox("show insignificant entities", &showInsignificant);
		auto view1 = context->getReg().view<transformComponent>();
		for (auto& e : view1)
			drawEntityUI(entity(e, context));
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selectedEntity = entity();
		ImGui::End();

		ImGui::Begin("Properties");
		if (!context->getReg().valid((entt::entity)selectedEntity)) selectedEntity = entity();
		if (selectedEntity) drawComponentUI(selectedEntity);
		ImGui::End();
	}

	void scenePanel::drawEntityUI(entity e)
	{
		auto& nc = e.getComponent<tagComponent>();
		if (!nc.significant && !showInsignificant) return;
		ImGuiTreeNodeFlags flags = ((e == selectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, nc.name.c_str());
		if (ImGui::IsItemClicked())
		{
			selectedEntity = e;
		}
		bool entityDelete = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("delete entity"))
				entityDelete = true;
			ImGui::EndPopup();
		}
		if (opened) ImGui::TreePop();
		if (entityDelete)
		{
			if (selectedEntity == e) selectedEntity = entity();
			context->destroyEntity(e);
		}
		if (saving)
		{
			ImGui::Begin("Save Scene");
			ImGui::InputText("Path", savepath, 1024);
			if (ImGui::Button("Save to path"))
			{
				serializer seri(context);
				seri.serialize(savepath);
				saving = false;
			}
			if (ImGui::Button("save to autosave"))
			{
				serializer seri(context);
				seri.serialize(AUTOSAVE_PATH);
				saving = false;
			}
			ImGui::End();
		}
		if (loading)
		{
			ImGui::Begin("Load Scene");
			ImGui::InputText("Path", loadpath, 1024);
			if (ImGui::Button("Load from path"))
			{
				serializer seri(context);
				seri.deserialize(loadpath);
				loading = false;
			}
			if (ImGui::Button("Load from autosave"))
			{
				serializer seri(context);
				seri.deserialize(AUTOSAVE_PATH);
				loading = false;
			}
			ImGui::End();
		}
	}
	void scenePanel::drawComponentUI(entity e)
	{
		if (ImGui::Button("Add Component", ImVec2(100.0f, 30.0f)))
			ImGui::OpenPopup("Add Component");
		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("camera"))
			{
				selectedEntity.addComponent<cameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("2D sprite"))
			{
				selectedEntity.addComponent<spriteRenderComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("mesh"))
			{
				selectedEntity.addComponent<meshComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("light"))
			{
				selectedEntity.addComponent<lightComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		// base component, everyone has these
		if (e.hasComponent<tagComponent>())
		{
			auto& comp = e.getComponent<tagComponent>();
			std::ostringstream ss;
			ss << "ID: " << (uint32_t)comp.ID;
			ImGui::Text(ss.str().c_str());
			char buf1[1024];
			memset(buf1, 0, 1024);
			memcpy(buf1, comp.tag.c_str(), 1024);
			if (ImGui::InputText("tag", buf1, sizeof(buf1)))
				comp.tag = std::string(buf1);
			char buf2[1024];
			memset(buf2, 0, 1024);
			memcpy(buf2, comp.name.c_str(), 1024);
			if (ImGui::InputText("name", buf2, sizeof(buf2)))
				comp.name = std::string(buf2);
			ImGui::Checkbox("is significant", &(comp.significant));
		}
		if (e.hasComponent<transformComponent>())
		{
			auto& comp = e.getComponent<transformComponent>();
			triValuePanel("position", comp.translate);
			glm::vec3 scale = comp.scale;
			triValuePanel("scale", scale);
			if (scale[0] < 0.01f) scale[0] = 0.01f;
			if (scale[1] < 0.01f) scale[1] = 0.01f;
			if (scale[2] < 0.01f) scale[2] = 0.01f;
			comp.scale = scale;
			triValuePanel("rotation", comp.rotation);

			int temp = comp.subjectTo;
			if (ImGui::InputInt("attach to", &temp)) comp.subjectTo = temp;

			//ImGui::DragFloat3("position", glm::value_ptr(comp.translate));
			//ImGui::DragFloat3("scale", glm::value_ptr(scale));
			//ImGui::DragFloat3("rotation", glm::value_ptr(comp.rotation));
		}

		// special components
		const ImGuiTreeNodeFlags treeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (e.hasComponent<cameraComponent>())
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(cameraComponent).hash_code(), treeFlag, "camera");
			bool componentDelete = false;
			ImGui::SameLine();
			if (ImGui::Button("remove component")) componentDelete = true;
			if (opened)
			{
				auto& comp = e.getComponent<cameraComponent>();

				char* typeTag[] = {"orthographic camera", "perspective camera"};
				if (ImGui::BeginCombo("Projection", typeTag[(int)comp.cam.type]))
				{
					for (int i = 0; i < 2; i++)
					{
						bool selected = (int)comp.cam.type == i;
						if (ImGui::Selectable(typeTag[i], selected))
							comp.cam.type = (sceneCamera::camType)i;
						if (selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				if (comp.fixRatio)
					ImGui::DragFloat("Ratio", &comp.cam.aspectRatio);

				if ((int)comp.cam.type == 0) // orthographic
				{
					ImGui::DragFloat("Size", &comp.cam.orthographicSize);
					ImGui::DragFloat("Near", &comp.cam.orthographicNear);
					ImGui::DragFloat("Far", &comp.cam.orthographicFar);
					comp.cam.recalculate();
				}
				if ((int)comp.cam.type == 1) // perspective
				{
					ImGui::DragFloat("field of view", &comp.cam.perspectiveFOV);
					ImGui::DragFloat("Near", &comp.cam.perspectiveNear);
					ImGui::DragFloat("Far", &comp.cam.perspectiveFar);
					comp.cam.recalculate();
				}
				ImGui::TreePop();
			}
			if (componentDelete) e.removeComponent<cameraComponent>();
		}
		if (e.hasComponent<spriteRenderComponent>())
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(spriteRenderComponent).hash_code(), treeFlag, "sprite");
			bool componentDelete = false;
			ImGui::SameLine();
			if (ImGui::Button("remove component")) componentDelete = true;
			if (opened)
			{
				auto& comp = e.getComponent<spriteRenderComponent>();
				ImGui::ColorEdit4("color mask", glm::value_ptr(comp.color));
				glm::vec2 tempCoord1 = comp.coord1;
				if (ImGui::DragFloat2("coord1", glm::value_ptr(tempCoord1)))
				{
					if (tempCoord1[0] < 0) tempCoord1[0] = 0;
					if (tempCoord1[1] < 0) tempCoord1[1] = 0;
					if (tempCoord1[0] > 1) tempCoord1[0] = 1;
					if (tempCoord1[1] > 1) tempCoord1[1] = 1;
					comp.coord1 = tempCoord1;
				}
				glm::vec2 tempCoord2 = comp.coord2;
				if (ImGui::DragFloat2("coord2", glm::value_ptr(tempCoord2)))
				{
					if (tempCoord2[0] < 0) tempCoord2[0] = 0;
					if (tempCoord2[1] < 0) tempCoord2[1] = 0;
					if (tempCoord2[0] > 1) tempCoord2[0] = 1;
					if (tempCoord2[1] > 1) tempCoord2[1] = 1;
					comp.coord2 = tempCoord2;
				}
				glm::vec2 tempCoord3 = comp.coord3;
				if (ImGui::DragFloat2("coord3", glm::value_ptr(tempCoord3)))
				{
					if (tempCoord3[0] < 0) tempCoord3[0] = 0;
					if (tempCoord3[1] < 0) tempCoord3[1] = 0;
					if (tempCoord3[0] > 1) tempCoord3[0] = 1;
					if (tempCoord3[1] > 1) tempCoord3[1] = 1;
					comp.coord3 = tempCoord3;
				}
				glm::vec2 tempCoord4 = comp.coord4;
				if (ImGui::DragFloat2("coord4", glm::value_ptr(tempCoord4)))
				{
					if (tempCoord4[0] < 0) tempCoord4[0] = 0;
					if (tempCoord4[1] < 0) tempCoord4[1] = 0;
					if (tempCoord4[0] > 1) tempCoord4[0] = 1;
					if (tempCoord4[1] > 1) tempCoord4[1] = 1;
					comp.coord4 = tempCoord4;
				}
				char buf[1024];
				memset(buf, 0, 1024);
				memcpy(buf, comp.texPath.c_str(), 1024);
				if (ImGui::InputText("texture Path", buf, sizeof(buf)))
				{
					comp.texPath = buf;
				}
				ImGui::TreePop();
			}
			if (componentDelete) e.removeComponent<spriteRenderComponent>();
		}
		if (e.hasComponent<meshComponent>())
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(meshComponent).hash_code(), treeFlag, "mesh");
			bool componentDelete = false;
			ImGui::SameLine();
			if (ImGui::Button("remove component")) componentDelete = true;
			if (opened)
			{
				auto& comp = e.getComponent<meshComponent>();

				int ind = 0;
				int pointDelete = -1;
				bool viewPoints = ImGui::TreeNodeEx((void*)typeid(meshComponent::pointInfo).hash_code(), treeFlag, "points");
				ImGui::SameLine();
				if (ImGui::Button("add point")) comp.addPoint();
				if (viewPoints)
				{
					for (meshComponent::pointInfo& item : comp.mesh)
					{
						ostringstream ss;
						ss << ind;
						bool viewPoint = ImGui::TreeNodeEx(("mesh point" + ss.str()).c_str(), treeFlag);
						ImGui::SameLine();
						if (ImGui::Button(("delete mesh point" + ss.str()).c_str())) pointDelete = ind;
						if (viewPoint)
						{
							glm::vec3 pos = item.position;
							glm::vec2 cor = item.texCoord;
							triValuePanel("position", pos);
							ImGui::DragFloat2("texture coord", glm::value_ptr(cor));
							if (cor.x > 1) cor.x = 1;
							if (cor.x < 0) cor.x = 0;
							if (cor.y > 1) cor.y = 1;
							if (cor.y < 0) cor.y = 0;

							comp.editPoint(ind, pos, cor);
							ImGui::TreePop();
						}
						ind++;
					}
					ImGui::TreePop();
				}

				ind = 0;
				int faceDelete = -1;
				bool viewFaces = ImGui::TreeNodeEx((void*)typeid(glm::vec3).hash_code(), treeFlag, "faces");
				ImGui::SameLine();
				if (ImGui::Button("add face")) comp.addFace();
				if (viewFaces)
				{
					for (meshComponent::faceInfo item : comp.faces)
					{
						ostringstream ss;
						ss << ind;
						bool viewFace = ImGui::TreeNodeEx(("mesh face" + ss.str()).c_str(), treeFlag);
						ImGui::SameLine();
						if (ImGui::Button(("delete mesh face" + ss.str()).c_str())) faceDelete = ind;
						if (viewFace)
						{
							glm::vec4 col = item.color;
							std::string tex = item.texPath;
							int pts[3] = { item.p1, item.p2, item.p3 };
							ImGui::ColorEdit4("color", glm::value_ptr(col));
							char buf[1024];
							memset(buf, 0, 1024);
							memcpy(buf, tex.c_str(), 1024);
							if (ImGui::InputText("texture Path", buf, sizeof(buf)))
								tex = buf;
							ImGui::DragInt3("texture coord", pts);

							comp.editFace(ind, col, tex, pts[0], pts[1], pts[2]);
							ImGui::TreePop();
						}
						ind++;
					}
					ImGui::TreePop();
				}

				if (faceDelete != -1) comp.rmFace(faceDelete);
				if (pointDelete != -1) comp.rmPoint(pointDelete);
				ImGui::TreePop();
			}
			if (componentDelete) e.removeComponent<meshComponent>();
		}
		if (e.hasComponent<lightComponent>())
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(lightComponent).hash_code(), treeFlag, "sprite");
			bool componentDelete = false;
			ImGui::SameLine();
			if (ImGui::Button("remove component")) componentDelete = true;
			if (opened)
			{
				auto& comp = e.getComponent<lightComponent>();
				float tempIntensity = comp.intensity;
				ImGui::DragFloat("light intensity", &tempIntensity);
				glm::vec4 tempColor = glm::vec4(comp.color, 1);
				ImGui::ColorEdit4("color mask", glm::value_ptr(tempColor));
				ImGui::DragFloat3("direction", glm::value_ptr(comp.direction));
				if (tempIntensity < 0) tempIntensity = 0;
				comp.intensity = tempIntensity;
				comp.color = tempColor;
				ImGui::TreePop();
			}
			if (componentDelete) e.removeComponent<spriteRenderComponent>();
		}
		if (e.hasComponent<nativeScriptComponent>())
		{
			auto& comp = e.getComponent<nativeScriptComponent>();
		}
	}
}