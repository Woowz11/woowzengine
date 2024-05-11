#include <string>
#include <map>
#include "RenderElement.h"
#include "Color.h"
#include "Vector2.h"
#include "l_Sprite.h"
#pragma once
using namespace std;

struct Scene {
public:
	string name;
	map<string, l_Sprite> sprites;
	Color BackgroundColor = Color(0,0,0,255);
	string windowid = "";
	Vector2 CameraPosition = Vector2(0, 0);

	void SetSprite(l_Sprite sprite) {
		sprites[sprite.id] = sprite;
	}

	Scene(string name = "new scene") : name(name) {}
};
