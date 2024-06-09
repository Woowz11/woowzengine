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

	void SetR(int i) {
		if (i < 0 || i>255) {
			PW("'Red' in color cannot be <0 and >255! Color must be an integer! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0015");
		}
		else {
			r = i;
		}
	}

	void SetG(int i) {
		if (i < 0 || i>255) {
			PW("'Green' in color cannot be <0 and >255! Color must be an integer! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0015");
		}
		else {
			g = i;
		}
	}

	void SetB(int i) {
		if (i < 0 || i>255) {
			PW("'Blue' in color cannot be <0 and >255! Color must be an integer! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0015");
		}
		else {
			b = i;
		}
	}

	void SetA(int i) {
		if (i < 0 || i>255) {
			PW("'Alpha' in color cannot be <0 and >255! Color must be an integer! Color.ToCPP(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + ")", "LW0015");
		}
		else {
			a = i;
		}
	}

	l_Color Invert() {
		return l_Color(255-r,255-g,255-b,a);
	}

	l_Color InvertAll() {
		return l_Color(255 - r, 255 - g, 255 - b, 255 - a);
	}

	l_Color Gray() {
		float r_ = (float(r) / 255) * 0.299f;
		float g_ = (float(g) / 255) * 0.587f;
		float b_ = (float(b) / 255) * 0.114f;
		int c = int(round((r_+g_+b_)*255));
		return l_Color(c,c,c,a);
	}

	l_Color Contrast(float power = 1.5) {
		int r_ = max(0, min(255, int(round(float(r - 128) * power + 128.0f))));
		int g_ = max(0, min(255, int(round(float(g - 128) * power + 128.0f))));
		int b_ = max(0, min(255, int(round(float(b - 128) * power + 128.0f))));
		return l_Color(r_, g_, b_, a);
	}

	int GetR() {
		return r;
	}

	int GetG() {
		return g;
	}

	int GetB() {
		return b;
	}

	int GetA() {
		return a;
	}

	Color ToCPP() {
		return Color(r, g, b, a);
	}

	l_Color(int r = 0, int g = 0, int b = 0, int a = 255) { SetR(r); SetG(g); SetB(b); SetA(a); }
};