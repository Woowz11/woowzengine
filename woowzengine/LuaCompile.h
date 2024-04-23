#include <sol/sol.hpp>
#include <mutex>
#include <any>
#pragma once
using namespace std;

void StartFunction(sol::function func, list<any> params);
sol::object AnyToObject(any obj);
void CompileScript(string Path);
void LuaCompile();
string ToString(sol::object text);