#pragma once
#include "scene/sceneall.h"
#include "application.h"

namespace tengine
{
	class scenePanel
	{
	public:
		scenePanel();
		scenePanel(scene* sce);
		void setContext(scene* sce);
		void onImguiRender();

	private:
		bool showInsignificant = false;
		void drawEntityUI(entity e);
		void drawComponentUI(entity e);
		scene* context = 0;
		entity selectedEntity;
		bool loading = false;
		bool saving = false;
		char savepath[1024];
		char loadpath[1024];
	};
}