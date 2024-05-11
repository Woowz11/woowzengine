#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include "Base.h"
#include "Cycles.h"
#include "OpenGame.h"
#include "GLFW.h"

using namespace std;

std::mutex mtx;

int Time = 0;

int GetActiveTime() {
	return Time;
}

void SetCycleEngine() {
	while (true) {
		auto start = std::chrono::high_resolution_clock::now();
		Time = Time + 1;
		Cycle();
		auto end = std::chrono::high_resolution_clock::now();
		auto needwait = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::this_thread::sleep_for(std::chrono::milliseconds(16) - needwait);
	}
}

void SetCycleFunction(sol::function func, int milisec) {
	auto cycle = [func, milisec]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
			mtx.lock();
			try {
				func();
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "LUA");
			}
			mtx.unlock();
		}
		};
	std::thread thread(cycle);
	thread.detach();
}