#include <string>
#include "Texture.h"
#pragma once
using namespace std;

struct RenderElement {
public:
	string shader;
	Texture texture;
	string name;

	RenderElement(string name = "new element") : name(name) {}
};
