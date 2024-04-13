#include <string>
#include <map>
#include "RenderElement.h"
#pragma once
using namespace std;

struct Scene {
public:
	string name;
	map<string, RenderElement> elements;

	Scene(string name = "new scene") : name(name) {}
};
