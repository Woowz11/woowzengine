#include <lua/lua.hpp>
#include <sol/sol.hpp>
#pragma once

int GetActiveTime();
int GetFPS();
void SetDTFunction(sol::function func);
void SetCycleEngine();
void CyclePerSecond();
void SetCycleFunction(sol::function func, int milisec);
void SetRepeatFunction(sol::function func, int count, int milisec);