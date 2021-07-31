#pragma once
#include "editor_core.h"
#include "tengine_all.h"
#include "scenePanel.h"

class editor2d : public t_layer
{
public:
	editor2d();
	~editor2d();
	void onAttach() override;
	void onUpdate(float deltaTime) override;
	void onEvent(t_event& e) override;
	void guiRender() override;
	glm::vec2 convertMousePos(int mod = 0);
private:
	struct profileTime
	{
		profileTime(long long t, const char* n)
			: time(t), name(n)
		{
			
		}
		long long time;
		const char* name;
	};
	std::vector<profileTime> profiles;

	// camera
	glm::vec3 transform = glm::vec3(1);
	glm::vec3 colorVec = glm::vec3(1);
	glm::vec2 lighPos = glm::vec3(0);
	glm::vec2 viewportSize;

	// editor
	frameBuffer* fb = 0;
	bool loading = false;
	bool saving = false;
	char savepath[1024];
	char loadpath[1024];

	// scene
	scene* activeScene = 0;
	entity* cameraEntity;
	scenePanel sp;


};