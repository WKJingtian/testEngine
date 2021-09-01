#pragma once
#include "scene.h"

namespace tengine
{
	class serializer
	{
	public:
		serializer(scene* sce);

		void serialize(const std::string path);
		void deserialize(const std::string path);
		void renameSave(const char* newPath, const char* oldPath);
	private:
		scene* target;
	};
}