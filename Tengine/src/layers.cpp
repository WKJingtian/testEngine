#include "tpch.h"
#include "layer/layerall.h"

#include "imgui/imgui.h"
#include "application.h"
#include "tengine_core.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

#include "input.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/examples/imgui_impl_glfw.h"

namespace tengine
{

	// layer.h
	t_layer::t_layer(const std::string& n) : name(n) {}
	t_layer::~t_layer() {}

	// layerStack.h
	layerStack::layerStack() { }
	layerStack::~layerStack()
	{
		for (t_layer* l : lay) delete l;
	}
	void layerStack::pushLayer(t_layer* l)
	{
		lay.emplace(lay.begin() + insert, l);
		insert++;
	}
	void layerStack::pushOverlay(t_layer* l)
	{
		lay.emplace_back(l);
	}
	void layerStack::popLayer(t_layer* l)
	{
		auto it = std::find(lay.begin(), lay.end(), l);
		if (it != lay.end())
		{
			lay.erase(it);
			insert--;
		}
	}
	void layerStack::popOverlay(t_layer* l)
	{
		auto it = std::find(lay.begin(), lay.end(), l);
		if (it != lay.end())
		{
			lay.erase(it);
		}
	}
	// imguiLayer.h
	imguiLayer::imguiLayer()
		:t_layer("imguiLayer") {}
	imguiLayer::~imguiLayer() {}
	void imguiLayer::onAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGuiStyle& sty = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			sty.WindowRounding = 0.0f;
			sty.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* win = static_cast<GLFWwindow*>(application::getApp().getwin().getWindow());
		ImGui_ImplGlfw_InitForOpenGL(win, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void imguiLayer::onDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void imguiLayer::onEvent(t_event& e)
	{
		//ImGuiIO& io = ImGui::GetIO();
		//e.handeled |= e.isCategory(ecMouse) & io.WantCaptureMouse;
		//e.handeled |= e.isCategory(ecKeyboard) & io.WantCaptureKeyboard;
	}
	void imguiLayer::begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void imguiLayer::end()
	{
		ImGuiIO& io = ImGui::GetIO();
		application& app = application::getApp();
		io.DisplaySize = ImVec2((float)app.getwin().getWidth(), (float)app.getwin().getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* cur = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(cur);
		}
	}
	void imguiLayer::guiRender()
	{
		//bool show = true;
		//ImGui::ShowDemoWindow(&show);
		//
	}
}