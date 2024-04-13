#include "Color.h"
#include "Vector2.h"
#pragma once

struct Vertex {
public:
	Vector2 position;
	Color color;

	Vertex(Vector2 v2, Color c) : position(v2), color(c) {}
};
