#include "Color.h"
#include "Vector2.h"
#pragma once

struct Vertex {
public:
	Vector2 position = Vector2(0,0);
	Color color = Color(0,0,0,255);

	Vertex() {}
	Vertex(Vector2 v2, Color c) : position(v2), color(c) {}
};
