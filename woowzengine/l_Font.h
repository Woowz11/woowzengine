#pragma once

class l_Font {
public:
	string id = "default";
	string image = "";

	l_Font() {}
	l_Font(string id, string image) : id(id), image(image) {}
};