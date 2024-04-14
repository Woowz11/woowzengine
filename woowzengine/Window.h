#include <string>
#include <GLFW/glfw3.h>
#include <sol/sol.hpp>
#include "Scene.h"
#pragma once
using namespace std;

struct Window {
public:
	string id = "new window";
	GLFWwindow* glfw = nullptr;
	Scene scene;
	float scale = 1;
	int StartSizeX = -1; int StartSizeY = -1;
	bool AutoResize = false;
	sol::function WindowClosed = sol::nil;
	sol::function WindowKeyPress = sol::nil;
	sol::function WindowKeyRelease = sol::nil;
	sol::function WindowKeyRepeat = sol::nil;

	Window(string id = "new window", GLFWwindow* glfw = nullptr) : id(id), glfw(glfw) {}
};
