#include "editor2d.h"
#include "imgui/imgui.h"
#include "tengine_core.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

editor2d::editor2d()
{
	// initialize graphic stuff
	renderCommand::init();
	renderer2D::init();
	renderer3D::init();

	// profiling
	profiles.push_back(profileTime(0, "on Update"));
}

editor2d::~editor2d()
{
	delete fb;
}

void editor2d::onAttach()
{
	// frame buffer
	frameBufferProp p;
	p.width = 1280;
	p.height = 720;
	fb = frameBuffer::create(p);

	// entity system
	activeScene = new scene();
	cameraEntity = activeScene->createEntity();
	cameraEntity->addComponent<cameraComponent>(sceneCamera());
	cameraEntity->addComponent<nativeScriptComponent>(nativeScriptComponent()).bind<cameraController>();
	sp.setContext(activeScene);
}

void editor2d::onUpdate(float deltaTime)
{
	// set up time monitor
	timeStamp ts1;
	ts1.init();

	// resize
	if (frameBufferProp prop = fb->prop;
		viewportSize.x > 0 && viewportSize.y > 0 &&
		(viewportSize.x != prop.width || viewportSize.y != prop.height))
	{
		// resize的效果有点奇怪
		//fb->resize(viewportSize.x, viewportSize.y);
		activeScene->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
	}

	// set up frame buffer
	fb->bind();

	// test mouse position calculation
	lighPos = glm::vec2(input::getmPos().first, input::getmPos().second);

	// prepare the renderer
	renderCommand::setClear(glm::vec4(0, 0, 0.3f, 1));
	glClearDepth(1.0f);
	renderCommand::clear();

	// render the scene
	activeScene->onUpdate(deltaTime);

	// clean up
	fb->unbind();
	profiles[0].time = ts1.reportDelta();
}

glm::vec2 editor2d::convertMousePos(int mod)
{
	float x = input::getmx();
	float y = input::getmy();
	uint32_t w = application::getApp().getwin().getWidth();
	uint32_t h = application::getApp().getwin().getHeight();
	if (mod == 0)
	{
		float yy = -1 * (y - (h / 2.0f)) * (cameraEntity->getComponent<cameraComponent>().cam.orthographicSize) / (h / 2.0f);
		float xx = (x - (w / 2.0f)) * (cameraEntity->getComponent<cameraComponent>().cam.orthographicSize
			* cameraEntity->getComponent<cameraComponent>().cam.aspectRatio) / (w / 2.0f);
		return glm::vec2(xx, yy) + glm::vec2(cameraEntity->getComponent<transformComponent>().translate[0],
			cameraEntity->getComponent<transformComponent>().translate[1]);
	}
	else
	{
		float yy = -1 * (y - (h / 2.0f)) / (h / 2.0f);
		float xx = (x - (w / 2.0f)) / (w / 2.0f);
		return glm::vec2(xx, yy) + glm::vec2(cameraEntity->getComponent<transformComponent>().translate[0],
			cameraEntity->getComponent<transformComponent>().translate[1]);
	}
}

void editor2d::onEvent(t_event& e)
{
	//t_dispatcher dispatcher(e);
	//dispatcher.dispatch<keyTypedEvent>(BIND_EVENT(editor2d::onKeyTyped));
	//dispatcher.dispatch<keyPressedEvent>(BIND_EVENT(editor2d::onKeyPressed));
}

void editor2d::guiRender()
{
	// the whole dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);
	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Close"))
				application::getApp().onEvent(windowCloseEvent());
			if (ImGui::MenuItem("new scene"))
				activeScene->destroyAll();
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

		ImGui::EndMenuBar();
	}

	// profile
	ImGui::Begin("debug panel");
	for (auto& result : profiles)
	{
		char tmp[1024];
		strcpy(tmp, result.name);
		strcat(tmp, " used %d nano sec");
		ImGui::Text(tmp, result.time);

		activeScene->lightDebug();
	}
	ImGui::End();

	// viewport
	ImGui::Begin("Viewport");
	viewportSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
	uint32_t texID = fb->colorAttachment;
	ImGui::Image((void*)(uint64_t)texID, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	// scene interaction
	sp.onImguiRender();

	ImGui::End();

	// save load
	if (saving)
	{
		ImGui::Begin("Save Scene");
		ImGui::InputText("Path", savepath, 1024);
		if (ImGui::Button("Save to path"))
		{
			serializer seri(activeScene);
			seri.serialize(savepath);
			saving = false;
		}
		if (ImGui::Button("save to autosave"))
		{
			serializer seri(activeScene);
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
			serializer seri(activeScene);
			seri.deserialize(loadpath);
			loading = false;
		}
		if (ImGui::Button("Load from autosave"))
		{
			serializer seri(activeScene);
			seri.deserialize(AUTOSAVE_PATH);
			loading = false;
		}
		ImGui::End();
	}
}