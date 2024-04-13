#include <string>
#include "Base.h"
#pragma once
using namespace std;

struct Texture {
public:
	string path;

	Texture(string path = GetSessionInfo("SourcePath")+"engine/error.png") : path(path) {}
};
