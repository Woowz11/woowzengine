#include <algorithm>
#pragma once

struct Color {
public:
	int r; int g; int b; int a;

	float GetR() const {
		return (float)r / 255;
	}
	float GetG() const {
		return (float)g / 255;
	}
	float GetB() const {
		return (float)b / 255;
	}
	float GetA() const {
		return (float)a / 255;
	}

	Color(int r = 0, int g = 0, int b = 0, int a = 255) : r(std::clamp(r, 0, 255)), g(std::clamp(g, 0, 255)), b(std::clamp(b, 0, 255)), a(std::clamp(a, 0, 255)) {}
};