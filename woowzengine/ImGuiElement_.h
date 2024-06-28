#pragma once
#include <sol/sol.hpp>
#include "l_Color.h"

using namespace std;

struct ImGuiElement_ {
public:
	string windowid = "";
	string id = "";
	string type = "Text";

	string text = "New Element";
	l_Color color = l_Color(255,255,255);
	sol::function func;

	ImGuiElement_() {}
	ImGuiElement_(string id, string windowid, string type) : id(id), windowid(windowid), type(type) {}
};