#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include "Base.h"
#include "Cycles.h"
#include "OpenGame.h"
#include "GLFW.h"

using namespace std;

std::mutex mtx;

void SetCycleEngine() {
	while (true) {
		Cycle();
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