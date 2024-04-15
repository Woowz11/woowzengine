#include <string>
#include <map>
#include "RenderElement.h"
#include "Color.h"
#pragma once
using namespace std;

struct Scene {
public:
	string name;
	map<string, RenderElement> elements;
	Color BackgroundColor = Color(0,0,0,255);
	string windowid = "";

	Scene(string name = "new scene") : name(name) {}
};
