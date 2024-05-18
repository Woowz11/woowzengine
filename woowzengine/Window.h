#include <string>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <sol/sol.hpp>
#include <map>
#include <list>
#pragma once
using namespace std;

struct Window {
public:
	string id = "new window";
	GLFWwindow* glfw = nullptr;
	string scene = "";
	float scale = 1;
	int StartSizeX = -1; int StartSizeY = -1;
	int Transparency = 255;
	bool AutoResize = false;
	unsigned int Buffer = 0;
	unsigned int Arrays = 0;
	map<string, GLuint> Shaders = {};
	map<string, map<string, int>> Uniforms = {};
	sol::function WindowClosed = sol::nil;
	sol::function WindowKeyPress = sol::nil;
	sol::function WindowKeyRelease = sol::nil;
	sol::function WindowKeyRepeat = sol::nil;
	sol::function WindowMousePress = sol::nil;
	sol::function WindowMouseRelease = sol::nil;
	sol::function WindowMouseRepeat = sol::nil;

	Window(string id = "new window", GLFWwindow* glfw = nullptr) : id(id), glfw(glfw) {}
};
