#include <string>
#include <vector>
#include "OpenGame.h"
#include "RenderElement.h"
#include "Color.h"
#include "Vector2.h"
#include "l_Sprite.h"
#pragma once
using namespace std;

struct Scene {
public:
	string name;
	Color BackgroundColor = Color(0,0,0,255);
	string windowid = "";
	Vector2 CameraPosition = Vector2(0, 0);
	float CameraZoom = 1;

	Scene(string name = "new scene") : name(name) {}
};
