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
sol::function DTFunction;

int Time = 0;
int FPS = -1;
int FPS_ = -1;

void SetDTFunction(sol::function func) {
	DTFunction = func;
}

int GetActiveTime() {
	return Time;
}

int GetFPS() {
	return FPS;
}

void SetCycleEngine() {
	while (true) {
		auto start = std::chrono::high_resolution_clock::now();
		Cycle();
		if (DTFunction) {
			try {
				DTFunction();
			}
			catch (const sol::error& e) {
				string what = e.what();
				PE(what, "LUA");
			}
		}
		Time++;
		FPS_++;
		auto end = std::chrono::high_resolution_clock::now();
		auto needwait = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::this_thread::sleep_for(std::chrono::milliseconds(16) - needwait);
	}
}

void CyclePerSecond() {
	auto cycle = []() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			mtx.lock();
			
			FPS = FPS_;
			FPS_ = 0;

			mtx.unlock();
		}
		};
	std::thread thread(cycle);
	thread.detach();
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