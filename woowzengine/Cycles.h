#include <lua/lua.hpp>
#include <sol/sol.hpp>
#pragma once

void SetCycleEngine();
void SetCycleFunction(sol::function func, int milisec);