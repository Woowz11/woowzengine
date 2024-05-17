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
#include "WindowsElements.h"

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

void StopCycleEngine() {
}

void StartCycleEngine() {
	SetCycleEngine();
}

int targetFPS = 60;
std::chrono::milliseconds frameTime(1000 / (targetFPS * 2));
void SetTargetFPS(int newfps) {
	if (newfps <= 0) {
		PE("New fps target cannot be <= 0! SetTargetFPS("+to_string(newfps) + ")", "E0019");
	}
	else {
		targetFPS = newfps;
		frameTime = std::chrono::milliseconds(1000 / (targetFPS * 2));
	}
}

void SetCycleEngine() {
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while (!GetFatal()) {
		auto frameStart = std::chrono::high_resolution_clock::now();

		Cycle();
		if (DTFunction && DTFunction.valid()) {
			std::thread t([=]() {
				try {
					{
						std::unique_lock<std::mutex> lock(mtx);
						DTFunction();
					}
				}
				catch (const sol::error& e) {
					string what = e.what();
					PE(what, "CLUA");
				}
				});
			t.detach();
		}

		Time++;
		FPS_++;

		auto frameEnd = std::chrono::high_resolution_clock::now();
		auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
		auto sleepTime = frameTime - frameDuration;
		if (sleepTime > std::chrono::milliseconds(0)) {
			std::this_thread::sleep_for(sleepTime);
		}
	}
}

void CyclePerSecond() {
	auto cycle = []() {
		while (!GetFatal()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			FPS = FPS_;
			FPS_ = 0;
		}
		};
	std::thread thread(cycle);
	thread.detach();
}

void SetCycleFunction(sol::function func, int milisec) {
	auto cycle = [func, milisec]() {
		while (!GetFatal()) {
			try {
				{
					std::unique_lock<std::mutex> lock(mtx);
					func();
				}
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "CLUA");
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
		}
		};
	std::thread thread(cycle);
	thread.detach();
}

void SetRepeatFunction(sol::function func,int count, int milisec) {
	auto cycle = [func, count, milisec]() {
		int i = 1;
		while (i < (count + 1)) {
			if (milisec > 0) { std::this_thread::sleep_for(std::chrono::milliseconds(milisec)); }
			try {
				{
					std::unique_lock<std::mutex> lock(mtx);
					func(i);
				}
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "RLUA");
			}
			i++;
		}
		};
	std::thread thread(cycle);
	thread.detach();
}