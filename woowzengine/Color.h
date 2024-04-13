#include <algorithm>
#pragma once

struct Color {
public:
	int r; int g; int b; int a;

	Color(int r = 0, int g = 0, int b = 0, int a = 255) : r(std::clamp(r, 0, 255)), g(std::clamp(g, 0, 255)), b(std::clamp(b, 0, 255)), a(std::clamp(a, 0, 255)) {}
};