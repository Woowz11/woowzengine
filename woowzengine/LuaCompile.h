#include <sol/sol.hpp>
#include <mutex>
#include <any>
#include "l_Color.h"
#pragma once
using namespace std;

l_Color ObjToColor(sol::object obj, l_Color ifnil = l_Color(0, 0, 0, 255));
string GetObjectType(sol::object obj);
void StartFunction(sol::function func, list<any> params);
sol::object AnyToObject(any obj);
void CompileScript(string Path);
void LuaCompile();
string ToString(sol::object text, string IfNil = "nil");
float ToNumber(sol::object obj);