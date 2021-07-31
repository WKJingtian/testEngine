#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
#include "graphic/graphicall.h"
#include "layer/layerall.h"
#include "renderer/renderall.h"
#include "util/utilall.h"

namespace tengine
{
	class application
	{
	public:
		application();
		virtual ~application();
		void run();
		void onEvent(t_event& e);
		void pushLayer(t_layer* l);
		void pushOverlay(t_layer* l);
		static inline application& getApp() { return *app_instance; }
		static inline float getDelta() { return deltaTime; }
		inline t_window& getwin() { return *win; }
	private:
		static float deltaTime;
		static timeStamp ts;
		static application* app_instance;

		bool onClose(windowCloseEvent& e);
		bool onResized(windowResizeEvent& e);
		bool running;
		bool minimize = false;
		layerStack ls;
		imguiLayer* guiLayer;
		std::shared_ptr<t_window> win;
	};
	application* createApplication();
}