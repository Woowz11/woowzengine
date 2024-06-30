#pragma once
using namespace std;

struct ImGuiWindow_ {
public:
	string id = "";
	string title = "";
	bool active = true;

	ImGuiWindow_() {};
	ImGuiWindow_(string id) : id(id) {}
};