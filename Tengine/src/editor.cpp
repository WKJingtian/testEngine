#include "editor_core.h"
#include "tengine_all.h"
#include "editor_all.h"

#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

using namespace tengine;
using namespace std;

#ifdef PLATFORM_WINDOWS
#define main main
int main()
{
	log("editor started", 1);
	//application app = application();
	//app.run();
	editorApp app;
	return app.run(0, 0);
	return 0;
}
#endif