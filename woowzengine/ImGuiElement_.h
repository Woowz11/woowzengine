#pragma once
#include <sol/sol.hpp>
#include "l_Color.h"

using namespace std;

struct ImGuiElement_ {
public:
	string windowid = "";
	string id = "";
	string type = "error";
	int zindex = 0;

	string text = "New Element";
	l_Color color = l_Color(255,255,255);
	sol::function func;
	bool hasbullet = false;
	bool sameline = false;
	string tooltip = "";

	float min = 0;
	float max = 100;
	int intvalue = 0;
	float floatvalue = 0;
	bool boolvalue = false;
	string stringvalue = "";

	void SetType(string t) {
		type = Lowercase(t);
		if (type == "text") {
			text = "New Text";
		}
		else if (type == "button" || type == "small button" || type == "left arrow button" || type == "right arrow button") {
			text = "New Button";
		}
		else if (type == "checkbox") {
			text = "New Checkbox";
		}
		else if (type == "list") {
			text = "New List";
			stringvalue = "Apple,Orange,Banana,Melon,Pineapple";
		}
		else if (type == "drag number" || type == "drag number 2" || type == "drag number 3" || type == "drag number 4") {
			text = "New Drag Number";
			floatvalue = 0.1f;
			intvalue = 2;
		}
		else if (type == "slider number" || type == "slider number 2" || type == "slider number 3" || type == "slider number 4") {
			text = "New Slider Number";
			intvalue = 2;
		}
		else if (type == "input text" || type == "input text multiline") {
			text = "New Input Text";
		}
		else if (type == "edit rgb" || type == "edit rgba" || type == "edit rgb extended" || type == "edit rgba extended") {
			text = "New Edit Color";
		}
	}

	ImGuiElement_() {}
	ImGuiElement_(string id, string windowid, string type, int z) : id(id), windowid(windowid), zindex(z) { SetType(type); }
};