#include <string>
#include "Texture.h"
#pragma once
using namespace std;

struct RenderElement {
public:
	string shader;
	Texture texture;
	string name;
	int ZIndex = 0;
	bool DontScale = false; /*��� UI ����������*/

	RenderElement(string name = "new element") : name(name) {}
};
