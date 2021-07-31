#pragma once
#include "tengine_core.h"
#include "event/eventall.h"
#include "layer/layerall.h"

namespace tengine
{
	class imguiLayer : public t_layer
	{
	public:
		imguiLayer();
		~imguiLayer();
		void guiRender() override;
		void onAttach() override;
		void onDetach() override;
		void onEvent(t_event& e) override;
		void begin();
		void end();
	private:

		float time = 0;
	};
}