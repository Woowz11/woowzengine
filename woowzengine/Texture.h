#include <string>
#include "Base.h"
#pragma once
using namespace std;

struct Texture {
public:
	string path;
	bool Linear = false;

	string sceneid = "";
	string spriteid = "";

	void SetID(string sid, string id) {
		sceneid = sid;
		spriteid = id;
	}

	void SetBlur(bool b) {
		Linear = b;
	}

	bool GetBlur() {
		return Linear;
	}

	string GetPath() {
		return path;
	}

	Texture(string path = GetSessionInfo("SourcePath")+"engine/default.png") : path(path) {}
};
