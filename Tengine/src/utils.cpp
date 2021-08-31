#include "tpch.h"
#include "util/utilall.h"
#include <windows.h>

namespace tengine
{
	// log

	void changeColor(int desiredColor)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
	}

	void log(const char* msg, int msg_level)
	{
		if (msg_level < LOG_LEVEL) return;
		if (msg_level == 0)
		{
			changeColor(3);
			std::cout << "TRACE:  ";
		}
		else if (msg_level == 1)
		{
			changeColor(2);
			std::cout << "MESSAGE:  ";
		}
		else if (msg_level == 2)
		{
			changeColor(6);
			std::cout << "WARNING:  ";
		}
		else if (msg_level == 3)
		{
			changeColor(4);
			std::cout << "ERROR:  ";
		}
		else
		{
			changeColor(1);
			std::cout << "UNKNOWN:  ";
		}
		changeColor(7);
		std::cout << msg << std::endl;
	}

	void log(std::string msg, int msg_level)
	{
		if (msg_level < LOG_LEVEL) return;
		if (msg_level == 0)
		{
			changeColor(3);
			std::cout << "TRACE:  ";
		}
		else if (msg_level == 1)
		{
			changeColor(2);
			std::cout << "MESSAGE:  ";
		}
		else if (msg_level == 2)
		{
			changeColor(6);
			std::cout << "WARNING:  ";
		}
		else if (msg_level == 3)
		{
			changeColor(4);
			std::cout << "ASSERT ERROR:  ";
		}
		else if (msg_level == 4)
		{
			changeColor(4);
			std::cout << "ERROR:  ";
		}
		else
		{
			changeColor(1);
			std::cout << "UNKNOWN:  ";
		}
		changeColor(7);
		std::cout << msg << std::endl;
	}

	template<typename T>
	bool t_assert(std::string str, T a, T b)
	{
		if (a != b) log(str, 4);
	}

	template<typename T>
	bool t_assert(const char* str, T a, T b)
	{
		if (a != b) log(str, 4);
	}

	void errout(const char* msg)
	{
		log(msg, 3);
		exit(EXIT_FAILURE);
	}

	void t_assert(bool val, int msg_level, const char* msg = "assertion: ")
	{
		if (val) return;
		std::cout << msg << std::endl;
		log("assertion failed!", msg_level);
		//if (msg_level >= 3) errout("");
	}

	// timer

	utiltimer::utiltimer()
	{
		begin = std::chrono::high_resolution_clock::now();
		my_name = "timer\0";
	}
	utiltimer::utiltimer(const char* name)
	{
		begin = std::chrono::high_resolution_clock::now();
		my_name = name;
	}
	utiltimer::~utiltimer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto nanoTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		std::cout << "TIMER " << my_name << ": " << nanoTime << std::endl;
	}

	timeStamp::timeStamp() { time = std::chrono::high_resolution_clock::now(); }
	void timeStamp::init() { time = std::chrono::high_resolution_clock::now(); }
	long long timeStamp::reportDelta()
	{
		auto end = std::chrono::high_resolution_clock::now();
		long long result = std::chrono::duration_cast<std::chrono::microseconds>(end - time).count();
		return result;
	}
	void timeStamp::printDeltatime(bool nanosec, const std::string& msg)
	{
		long long deltatime = reportDelta();
		if(nanosec) std::cout << msg << "report timer: " << deltatime << std::endl;
		else std::cout << msg << "report timer: " << NANO_TO_SEC << std::endl;
	}
}