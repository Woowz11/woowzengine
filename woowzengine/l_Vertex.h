#include "Vertex.h"
#include "l_Color.h"
#include "l_Vector2.h"
#pragma once

class l_Vertex {
public:
	l_Vertex() {}
	l_Vertex(l_Vector2 v, l_Color c) : Position(v), Color(c) {}
	l_Vertex(l_Vector2 v) : Position(v) {}

	l_Vector2 Position = l_Vector2(0, 0);
	l_Color Color = l_Color(255, 255, 255, 255);

	Vertex ToCPP() {
		return Vertex(Position.ToCPP(), Color.ToCPP());
	}
};