#include <string>
#include "l_Vector2.h"
#include "l_Color.h"
#include "Base.h"
#pragma once
using namespace std;

struct l_Sprite {
public:
	string id = "";
	string texture = GetSessionInfo("SourcePath") + "engine/default.png";
	string shader = "";
	string posshader = "";
	l_Vector2 position = l_Vector2(0,0);
	float angle = 0;
	l_Vector2 origin = l_Vector2(0.5, 0.5);
	l_Color color = l_Color(255,255,255,255);
	float zindex = 0;
	l_Vector2 size = l_Vector2(1,1);
	bool movewithcamera = false;
	bool Linear = false;
	bool FlipX = false;
	bool FlipY = false;

	l_Sprite() {}
	l_Sprite(string id) : id(id) {}
	l_Sprite(string id, string texture, l_Vector2 pos) : id(id),texture(texture),position(pos) {}
};
