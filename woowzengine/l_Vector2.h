#include "Vector2.h"
#pragma once

class l_Vector2 {
public:
	l_Vector2(float x = 0, float y = 0) : x(x), y(y) {}

	float x = 0;
	float y = 0;

	Vector2 ToCPP() {
		return Vector2(x, y);
	}
};