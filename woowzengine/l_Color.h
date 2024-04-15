#include "Base.h"
#include "Color.h"
#pragma once
using namespace std;

class l_Color {
public:
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;

	l_Color(int r = 0, int g = 0, int b = 0, int a = 255) : r(r), g(g), b(b), a(a) {}

	Color ToCPP() {
		int r_ = 0;
		int g_ = 0;
		int b_ = 0;
		int a_ = 255;
		if (r < 0 || r>255) { PW("'Red' in color cannot be <0 and >255! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0015"); }
		else {
			r_ = r;
		}
		if (g < 0 || g>255) { PW("'Green' in color cannot be <0 and >255! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0016"); }
		else {
			g_ = g;
		}
		if (b < 0 || b>255) { PW("'Blue' in color cannot be <0 and >255! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0017"); }
		else {
			b_ = b;
		}
		if (a < 0 || a>255) { PW("'Alpha' in color cannot be <0 and >255! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0018"); }
		else {
			a_ = a;
		}
		return Color(r_, g_, b_, a_);
	}
};