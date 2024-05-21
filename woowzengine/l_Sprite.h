#include <string>
#include "l_Vector2.h"
#include "l_Color.h"
#include "Texture.h"
#include "Base.h"
#pragma once
using namespace std;

struct l_Sprite {
public:
	string id = "";
	Texture texture = Texture(GetSessionInfo("SourcePath") + "engine/default.png");
	string shader = "default";
	l_Vector2 position = l_Vector2(0,0);
	float angle = 0;
	l_Vector2 origin = l_Vector2(0.5, 0.5);
	l_Color color = l_Color(255,255,255,255);
	float zindex = 0;
	float zindex_code = 0;
	int cout = 0;
	l_Vector2 size = l_Vector2(1,1);
	bool movewithcamera = false;
	bool autoresize = false;
	bool FlipX = false;
	bool FlipY = false;
	bool DontHide = false;

	l_Vector2 LT = l_Vector2(0, 0);
	l_Vector2 LB = l_Vector2(0, 0);
	l_Vector2 RT = l_Vector2(0, 0);
	l_Vector2 RB = l_Vector2(0, 0);

	l_Vector2 UVLT = l_Vector2(0, 1);
	l_Vector2 UVLB = l_Vector2(0, 0);
	l_Vector2 UVRT = l_Vector2(1, 1);
	l_Vector2 UVRB = l_Vector2(1, 0);

	string sceneid = "";

	l_Sprite() {}
	l_Sprite(string id) : id(id) {}
	l_Sprite(string id, string sceneid) : id(id),sceneid(sceneid) {}
};