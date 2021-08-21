#pragma once
#include "glfw/glfw3.h"
#include "window.h"
#include "renderer/renderall.h"

namespace tengine
{
	//void glfwErrorCallback(int e, const char* msg);
	//class winWindow : public t_window
	//{
	//	struct win_data
	//	{
	//		std::string title;
	//		unsigned int width, height;
	//		eventCallbackFn eventCallback;
	//		bool VSync;
	//	};
	//	win_data prop;
	//	openglContext* context;
	//public:
	//	winWindow(const windowProp& p);
	//	virtual ~winWindow();
	//	void onUpdate() override;
	//	inline unsigned int getWidth() const override { return prop.width; }
	//	inline unsigned int getHeight() const override { return prop.height; }
	//	inline void setEventCallback(const eventCallbackFn& cb) override
	//	{ prop.eventCallback = cb; }
	//	void setVSync(bool enable) override;
	//	bool isVSync() const override;
	//	inline void* getWindow() const override { return win; }
	//private:
	//	virtual void init(const windowProp& p);
	//	virtual void shutdown();
	//	GLFWwindow* win;
	//};
}