#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <map>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glad/gl.c>
#include "GLFW.h"
#include "Base.h"
#include "Easyer.h"
#include "LuaCompile.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Window.h"

using namespace std;

map<string, Window> Windows;
map<GLFWwindow*, string> Windows_2;
string MainWindow = "";

void StopGLFW() {
	glfwTerminate();
}

void GLFWInstall() {
	if (!glfwInit()) {
		PF("GLFW cannot be realized!","C0017");
	}
	glfwSetErrorCallback(PE_GLFW);
	P("GLFW", "GLFW Installed!");

	SetSessionInfo("MainWindow", "");
}

void PE_GLFW(int error, const char* desc) {
	string desc_ = desc;
	PE(desc_,"S"+to_string(error));
}

/*������*/

void RenderObjectTriangles(Vertex v1, Vertex v2, Vertex v3) {
	glBegin(GL_TRIANGLES);
	glColor4f(v1.color.r/255, v1.color.g / 255, v1.color.b / 255, v1.color.a / 255);
	glVertex2f(v1.position.x, v1.position.y);
	glColor4f(v2.color.r / 255, v2.color.g / 255, v2.color.b / 255, v2.color.a / 255);
	glVertex2f(v2.position.x, v2.position.y);
	glColor4f(v3.color.r / 255, v3.color.g / 255, v3.color.b / 255, v3.color.a / 255);
	glVertex2f(v3.position.x, v3.position.y);
	glEnd();
}

/*������*/

void Render() {
	for (auto const& [id, window] : Windows) {
		if (!glfwWindowShouldClose(window.glfw)) {
			glfwMakeContextCurrent(window.glfw);
			int version = gladLoadGL(glfwGetProcAddress);
			if (version == 0) {
				PF("OpenGL cannot be realized! Render()", "C0020");
			}

			int width, height;
			if (window.AutoResize) {
				glfwGetFramebufferSize(window.glfw, &width, &height);
			}
			else {
				width = window.StartSizeX;
				height = window.StartSizeY;
			}
			glViewport(0, 0, round(width * window.scale), round(height * window.scale));

			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			RenderObjectTriangles(Vertex(Vector2(-1, -1), Color(255, 0, 0)), Vertex(Vector2(-1,1), Color(0, 255, 0)), Vertex(Vector2(1,-1), Color(0, 0, 255)));
			glfwSwapBuffers(window.glfw);
			glfwPollEvents();
		}
		else {
			if (id == MainWindow) {
				Exit();
			}
			DestroyWindowGLFW(id);
		}
	}
}

/*��������� ���� ���� � ������ ��� ���*/
bool HasWindow(string id) {
	auto it = Windows.find(id);
	if (it != Windows.end()) {
		return true;
	}
	else {
		return false;
	}
}

/*������� ���� �������*/
void SetWindowToMain(string id) {
	if (HasWindow(id)) {
		MainWindow = id;
		SetSessionInfo("MainWindow",id);
	}
	else {
		PW("Window not found! SetWindowToMain('"+id+"')","W0005");
	}
}

/*����� ����� �������� ����*/
void SetWindowTitle(string id, string title) {
	glfwSetWindowTitle(GetWindowByID(id).glfw, StringToConstChar(title));
}

/*�������� ���� �� ����*/
Window GetWindowByID(string id) {
	auto it = Windows.find(id);
	if (it != Windows.end()) {
		return it->second;
	}
	else {
		PF("Window not found! GetWindowByID('" + id + "')", "C0019");
		return Window();
	}
}

/*�������� ���� �� GLFWwindow*/
string GetIDByWindow(GLFWwindow* window) {
	auto it = Windows_2.find(window);
	if (it != Windows_2.end()) {
		return it->second;
	}
	else {
		PF("Window not found! GetIDByWindow()", "C0019");
		return "";
	}
}

/*�������� ���� �� GLFWwindow*/
Window GetWindowByWindow(GLFWwindow* window) {
	return GetWindowByID(GetIDByWindow(window));
}

/*�������� ������ ����*/
void SetWindowSize(string id, bool thatY, int size) {
	int x = GetWindowSize(id, false), y = GetWindowSize(id, true);
	bool error = false;
	if (thatY) {
		if (size <= 0) { error = true; }
		y = size;
	}
	else {
		if (size <= 121) { error = true; }
		x = size;
	}
	if (error) {
		PE("Window size cannot be x<=121, y<=0! CreateWindow('" + id + "'," + to_string(thatY) + "," + to_string(size) + ")", "E0008");
	}
	else {
		glfwSetWindowSize(GetWindowByID(id).glfw,x,y);
	}
}

/*�������� ������ ����*/
int GetWindowSize(string id, bool thatY) {
	int x, y;
	glfwGetWindowSize(GetWindowByID(id).glfw, &x, &y);
	if (thatY) {
		return y;
	}
	else {
		return x;
	}
}

/*��������� ����������� OpenGL*/
void SetWindowAutosize(string id, bool b) {
	Window w = GetWindowByID(id);
	w.AutoResize = b;
	Windows[id] = w;
}

/*��������� �������*/
void SetWindowScale(string id, float scale) {
	if (scale <= 0) { PE("Scale cannot be <= 0! SetWindowScale('"+id+"',"+to_string(scale) + ")", "E0009"); }
	else {
		Window w = GetWindowByID(id);
		w.scale = scale;
		Windows[id] = w;
	}
}

/*��������� ������ ������ ���� ��� ���*/
void SetWindowResizable(string id, bool b) {
	glfwSetWindowAttrib(GetWindowByID(id).glfw,GLFW_RESIZABLE,b);
}

/*����� ��� �������� ����*/
void SetWindowClosedEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowClosed = f;
	Windows[id] = w;
}

/*������� ������� � ����*/
void SetWindowKPEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowKeyPress = f;
	Windows[id] = w;
}

/*������� ������� � ����*/
void SetWindowKREvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowKeyRelease = f;
	Windows[id] = w;
}

/*������� ������� � ����*/
void SetWindowKHEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowKeyRepeat = f;
	Windows[id] = w;
}

map<int, string> Keys = {
	{49,"1"},{50,"2"},{51,"3"},{52,"4"},{53,"5"},{54,"6"},{55,"7"},{56,"8"},{57,"9"},{48,"0"},
	{341,"ctrl"},{340,"shift"},{280,"caps"},{258,"tab"},{96,"tilda"},{256,"esc"},{343,"win"},{342,"alt"},{32,"space"},
	{344,"shift2"},{257,"enter"},{259,"backspace"},{45,"minus"},{61,"plus"},{92,"slash"},{283,"printscreen"},{260,"insert"},{268,"home"},{261,"delete"},
	{266,"pageup"},{267,"pagedown"},{-1,"notfound"},{265,"up"},{263,"left"},{264,"down"},{262,"right"},
	{290,"f1"},{291,"f2"},{292,"f3"},{293,"f4"},{294,"f5"},{295,"f6"},{296,"f7"},{297,"f8"},{298,"f9"},{299,"f10"},{300,"f11"},{301,"f12"},
	{282,"numlock"},{331,"numslash"},{332,"asterisk"},{333,"numminus"},{327,"num7"},{328,"num8"},{329,"num9"},{334,"numplus"},{324,"num4"},{325,"num5"},{326,"num6"},{321,"num1"},{322,"num2"},{323,"num3"},{335,"numenter"},{320,"num0"},{330,"numdelete"},
	{81,"q"},{87,"w"},{69,"e"},{82,"r"},{84,"t"},{89,"y"},{85,"u"},{73,"i"},{79,"o"},{80,"p"},{91,"leftbracket"},{93,"rightbracket"},
	{65,"a"},{83,"s"},{68,"d"},{70,"f"},{71,"g"},{72,"h"},{74,"j"},{75,"k"},{76,"l"},{59,"colon"},{39,"apostrophe"},
	{90,"z"},{88,"x"},{67,"c"},{86,"v"},{66,"b"},{78,"n"},{77,"m"},{44,"comma"},{46,"dot"},{47,"question"}
};

map<int, bool> PressedKeys = {};

/*�������� ������� �� ����*/
string GetKeyFromID(int i) {
	string key = "key_"+to_string(i);
	if (Keys.find(i) != Keys.end()) {
		return Keys.find(i)->second;
	}
	return key;
}

/*�������� � ������ ������ ������� �������*/
map<string, int> GetPressedKeys() {
	map<string, int> k = {};
	for (const auto& p : PressedKeys) {
		if (p.second == true) {
			k[GetKeyFromID(p.first)] = p.first;
		}
	}
	return k;
}

void KeyCallback(GLFWwindow* window_, int key_, int scancode, int action, int mods) {
	Window window = GetWindowByWindow(window_);
	if (action == GLFW_PRESS) {
		if (window.WindowKeyPress.valid()) {
			StartFunction(window.WindowKeyPress, GetKeyFromID(key_));
		}
		PressedKeys[key_] = true;
	}
	if (action == GLFW_RELEASE) {
		if (window.WindowKeyRelease.valid()) {
			StartFunction(window.WindowKeyRelease, GetKeyFromID(key_));
		}
		PressedKeys[key_] = false;
	}
	if (action == GLFW_REPEAT) {
		if (window.WindowKeyRepeat.valid()) {
			StartFunction(window.WindowKeyRepeat, GetKeyFromID(key_));
		}
	}
}

Window CreateWindowGLFW(string id, int sizex, int sizey, string title) {
	if (HasWindow(id)) { PF("Window with this id already exists! CreateWindow('" + id + "'," + to_string(sizex) + "," + to_string(sizey) + ",'" + title + "')","C0021"); return Window(); }
	else {
		if (sizex<=121 || sizey <= 0) { PF("Window size cannot be x<=121, y<=0! CreateWindow('" + id + "'," + to_string(sizex) + "," + to_string(sizey) + ",'" + title + "')", "C0022"); return Window(); }
		else {
			GLFWwindow* window = glfwCreateWindow(sizex, sizey, StringToConstChar(title), NULL, NULL);
			if (!window) {
				PF("Window could not be created! CreateWindow('" + id + "'," + to_string(sizex) + "," + to_string(sizey) + ",'" + title + "')", "C0018");
				return Window();
			}
			Window window_ = Window(id,window);
			window_.StartSizeX = sizex;
			window_.StartSizeY = sizey;
			Windows[id] = window_;
			Windows_2[window] = id;
			glfwSetKeyCallback(window, KeyCallback);

			P("WINDOW", "Window [" + id + "] created!");
			return window_;
		}
	}
}

void DestroyWindowGLFW(string id) {
	if (!HasWindow(id)) { PW("Window not found! DestroyWindowGLFW('"+id+"')","W0004"); }
	else {
		if (id == MainWindow) {
			Exit();
		}
		Window window = GetWindowByID(id);
		if (window.WindowClosed.valid()) {
			StartFunction(window.WindowClosed);
		}
		glfwDestroyWindow(window.glfw);
		P("WINDOW", "Window [" + id + "] destroyed!");
		Windows_2.erase(window.glfw);
		Windows.erase(id);
	}
}