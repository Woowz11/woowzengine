#include <string>
#include <vector>
#include "RenderElement.h"
#include "Color.h"
#include "Vector2.h"
#include "l_Sprite.h"
#pragma once
using namespace std;

struct Scene {
public:
	string name;
	vector<l_Sprite> sprites;
	Color BackgroundColor = Color(0,0,0,255);
	string windowid = "";
	Vector2 CameraPosition = Vector2(0, 0);
	float CameraZoom = 1;

	void SetSprite(l_Sprite sprite) {
		auto it = find_if(sprites.begin(), sprites.end(), [&](const l_Sprite& sprite_) {
			return sprite_.id == sprite.id;
		});

		*it = sprite;
	}

	l_Sprite Find(string id) {
		auto it = find_if(sprites.begin(), sprites.end(), [&](const l_Sprite& sprite_) {
			return sprite_.id == id;
		});

		if (it != sprites.end()) {
			return *it;
		}
		else {
			PE("No sprite found! GetSprite('" + name + "','" + id + "')", "E0022");
			return l_Sprite("");
		}
	}

	void NewSprite(l_Sprite sprite) {
		sprites.push_back(sprite);
	}

	Scene(string name = "new scene") : name(name) {}
};
