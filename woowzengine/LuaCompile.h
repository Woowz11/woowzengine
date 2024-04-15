#include <sol/sol.hpp>
#include <mutex>
#pragma once
using namespace std;

void StartFunction(sol::function func, string s = nullptr);
void CompileScript(string Path);
void LuaCompile();