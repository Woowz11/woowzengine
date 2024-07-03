#include <sol/sol.hpp>
#include <mutex>
#include <any>
#include "l_Color.h"
#include "WConst.h"
#pragma once
using namespace std;

l_Color ObjToColor(sol::object obj, l_Color ifnil = ErrorColor, bool ingoreerror = false);
string GetObjectType(sol::object obj);
void StartFunction(sol::function func, list<any> params);
sol::object AnyToObject(any obj);
void CompileScript(string Path);
void LuaCompile();
bool ToBool(sol::object obj, bool IfNil = false);
float ToFloat(sol::object obj, float IfNil = 0);
string ToString(sol::object text, string IfNil = "nil");
float ToNumber(sol::object obj);