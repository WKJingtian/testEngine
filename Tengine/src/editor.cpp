#include "tengine_all.h"

#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#ifdef PLATFORM_WINDOWS
#define main main
int main()
{
	tengine::log("editor started", 1);
	//application app = application();
	//app.run();
	tengine::editorApp app;
	return app.run(0, 0);
	return 0;
}
#endif