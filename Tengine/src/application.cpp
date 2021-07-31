#include "tpch.h"
#include "application.h"

namespace tengine
{
	application* application::app_instance = NULL;
	float application::deltaTime = 0;
	timeStamp application::ts = timeStamp();

	application::application()
	{
		if (app_instance == NULL) app_instance = this;
		else
		{
			log("multiple applications generated!", 3);
			exit(EXIT_FAILURE);
		}
		win.reset(t_window::create());
		win->setEventCallback(BIND_EVENT(application::onEvent));

		renderer::init();

		guiLayer = new imguiLayer();
		pushOverlay(guiLayer);
		running = true;
	}

	application::~application() {}
	
	void application::onEvent(t_event& e)
	{
		t_dispatcher disp(e);
		disp.dispatch<windowCloseEvent>(BIND_EVENT(application::onClose));
		disp.dispatch<windowResizeEvent>(BIND_EVENT(application::onResized));
		for (auto l = ls.end(); l != ls.begin(); )
		{
			(*--l)->onEvent(e);
			if (e.handeled) break;
		}
	}

	bool application::onClose(windowCloseEvent& e)
	{
		running = false;
		return true;
	}
	bool application::onResized(windowResizeEvent& e)
	{
		if (e.height == 0 || e.width == 0) minimize = true;
		else minimize = false;
		renderer::onResized(e.width, e.height);
		return false;
	}

	void application::run()
	{
		ts.init();
		while (running)
		{
			if (!minimize)
				for (t_layer* l : ls) l->onUpdate(deltaTime);
			guiLayer->begin();
			for (t_layer* l : ls) l->guiRender();
			guiLayer->end();
			win->onUpdate();
			deltaTime = (float)ts.reportDelta();
		}
	}

	void application::pushLayer(t_layer* l)
	{
		ls.pushLayer(l);
		l->onAttach();
	}

	void application::pushOverlay(t_layer* l)
	{
		ls.pushOverlay(l);
		l->onAttach();
	}
}