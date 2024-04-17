#include <list>
#include <map>
#include "Scene.h"
#include "l_Element.h"
#include "RenderElement.h"
#include "GLFW.h"
#include "l_Color.h"

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
	
	void AddElement(l_Element e) {
		/*if (Elements.find(e.ID) != Elements.end()) { PE("Such an element [" + e.ID + "] is already on the scene [" + ID + "]!", "L0017"); }
		else {
			P("SCENE", "Element [" + e.ID + "] added to scene[" + ID + "]");
			Elements[e.ID] = e;
			Update();
		}*/
	}

	void AddSprite(l_Sprite e) {
		if (Elements.find(e.id) != Elements.end()) { PE("Such an sprite [" + e.id + "] is already on the scene [" + ID + "]!", "L0017"); }
		else {
			P("SCENE", "Sprite [" + e.id + "] added to scene[" + ID + "]");
			Sprites[e.id] = e;
			Update();
		}
	}

	Scene ToCPP() {
		Scene s = Scene(ID);
		/*map<string, RenderElement> Elements_ = {};
		for (const auto& pair : Elements) {
			Elements_[pair.first] = static_cast<l_Element>(pair.second).ToCPP();
		}
		s.elements = Elements_;*/
		s.sprites = Sprites;
		s.windowid = windowid;
		s.BackgroundColor = BackgroundColor.ToCPP();
		return s;
	}
};