#include "editor_core.h"
#include "tengine_all.h"
#include "editor_all.h"

using namespace tengine;
using namespace std;
class editor : public application
{
public:
	editor() : application()
	{
		pushLayer(new editor2d());
	}
	~editor() {}

};

tengine::application* tengine::createApplication()
{
	return new editor();
}

#ifdef PLATFORM_WINDOWS

//extern tengine::application* tengine::createApplication();
int main(int argc, char** argv)
{
	log("editor started", 1);
	auto app = tengine::createApplication();
	app->run();
	delete app;
}

#endif