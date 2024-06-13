#include <string>
#include "l_Vector2.h"
#include "l_Color.h"
#include "Base.h"
#pragma once
using namespace std;

struct l_Text {
public:
	string id = "";
	string font = "default";
	string shader = "font";
	string text = "Default text!";
	bool Visible = true;
	bool mono = false;
	l_Vector2 position = l_Vector2(0, 0);
	float scale = 1;
	float height = 0;
	l_Color color = l_Color(255, 255, 255);

	string sceneid = "";

	l_Text(string id, string sceneid) : id(id), sceneid(sceneid) {}
	l_Text(string id, string sceneid, string text) : id(id), sceneid(sceneid), text(text) {}
};