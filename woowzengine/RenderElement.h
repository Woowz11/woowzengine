#include <string>
#include <list>
#include "Texture.h"
#include "Vertex.h"
#pragma once
using namespace std;

struct RenderElement {
public:
	string shader;
	Texture texture;
	string id;
	string type = "";
	std::list<Vertex> Vertexs = {};
	int ZIndex = 0;
	bool DontScale = false; /*Для UI интерфейса*/

	RenderElement(string id = "new element") : id(id) {}
};
