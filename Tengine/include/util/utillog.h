#pragma once
#include "tpch.h"
#include "tengine_core.h"

namespace tengine
{

#define LOG_LEVEL 0
	using namespace std;
	/*
		0 -> log out everything
		1 -> log out warning and error only
		2 -> log out error only
	*/
	void changeColor(int desiredColor);

	void log(const char* msg, int msg_level = 0);

	void log(std::string msg, int msg_level = 0);

	template<typename T>
	bool t_assert(std::string str, T a, T b);

	template<typename T>
	bool t_assert(const char* str, T a, T b);

	void t_assert(bool val, int msg_level, const char* msg);

	void errout(const char* msg);
}