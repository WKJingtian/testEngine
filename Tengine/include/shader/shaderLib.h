#pragma once
#include "shader.h"

namespace tengine
{
	class shaderLib
	{
	public:
		void add(const std::string& n, const std::shared_ptr<shader>& sha);
		std::unordered_map<std::string, std::shared_ptr<shader>> lib;
	};
}