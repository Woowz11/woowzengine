#include <string>
#include <GLFW/glfw3.h>
#include "Scene.h"
#pragma once
using namespace std;

struct Window {
public:
	string id = "new window";
	GLFWwindow* glfw = nullptr;
	Scene scene;
	bool AutoResize = false;

	Window(string id = "new window", GLFWwindow* glfw = nullptr) : id(id), glfw(glfw) {}
};
