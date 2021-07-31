#pragma once
#include "tpch.h"
#include "tengine_core.h"
#include <chrono>

namespace tengine
{
	class utiltimer
	{
		std::chrono::steady_clock::time_point begin;
		const char* my_name;
	public:
		utiltimer();
		utiltimer(const char* name);
		~utiltimer();
	};

	class timeStamp
	{
	public:
		timeStamp();
		void init();
		long long reportDelta();
		void printDeltatime(bool nanosec = true, const std::string& msg = "");
		std::chrono::steady_clock::time_point time;
	};
}