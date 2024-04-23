#include <list>
#include <map>
#include "Scene.h"
#include "l_Element.h"
#include "RenderElement.h"
#include "GLFW.h"
#include "l_Color.h"
#include "l_Vector2.h"
#include "l_Sprite.h"

#pragma once
using namespace std;

class l_Scene {
public:
	l_Scene() {}
	l_Scene(string id) : ID(id) {}
	string ID = "new scene";
	map<string, l_Element> Elements = {};
	map<string, l_Sprite> Sprites = {};
	string windowid = "";
	l_Color BackgroundColor = l_Color(0, 0, 0, 255);
	l_Vector2 CameraPosition = l_Vector2(0, 0);

	l_Color GetBackgroundColor() {
		return BackgroundColor;
	}

	void SetBackgroundColor(l_Color c) {
		BackgroundColor = c;
		Update();
	}

	void Update() {
		if (windowid != "") {
			SetWindowScene(windowid, ToCPP(), true);
		}
	}
	
	void SetCameraPosition(float f, bool thatx) {
		if (thatx) {
			CameraPosition = l_Vector2(f,CameraPosition.y);
		}
		else {
			CameraPosition = l_Vector2(CameraPosition.x, f);
		}
		Update();
	}

	l_Sprite GetSprite(string id) {
		return Sprites[id];
	}

	void AddSprite(l_Sprite& e) {
		if (Elements.find(e.id) != Elements.end()) { PE("Such an sprite [" + e.id + "] is already on the scene [" + ID + "]!", "L0017"); }
		else {
			P("SCENE", "Sprite [" + e.id + "] added to scene[" + ID + "]");
			Sprites[e.id] = e;
			e.sceneid = ID;
			Update();
		}
	}

	void UpdateSprite(l_Sprite e) {
		Sprites[e.id] = e;
		Update();
	}

	Scene ToCPP() {
		Scene s = Scene(ID);
		s.sprites = Sprites;
		s.windowid = windowid;
		s.BackgroundColor = BackgroundColor.ToCPP();
		s.CameraPosition = CameraPosition.ToCPP();
		return s;
	}
};