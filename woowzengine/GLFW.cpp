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

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Window.h"

using namespace std;

map<string, Window> Windows;
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

/*Рендер*/

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

/*Методы*/

void Render() {
	for (auto const& [id, window] : Windows) {
		if (!glfwWindowShouldClose(window.glfw)) {
			glfwMakeContextCurrent(window.glfw);
			int version = gladLoadGL(glfwGetProcAddress);
			if (version == 0) {
				PF("OpenGL cannot be realized! Render()", "C0020");
			}

			if (window.AutoResize) {
				int width, height;
				glfwGetFramebufferSize(window.glfw, &width, &height);
				glViewport(0, 0, width, height);
			}

			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			RenderObjectTriangles(Vertex(Vector2(0, 1), Color(255, 0, 0)), Vertex(Vector2(-1, -1), Color(0, 255, 0)), Vertex(Vector2(1, -1), Color(0, 0, 255)));
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

/*Проверяет есть окно в данных или нет*/
bool HasWindow(string id) {
	auto it = Windows.find(id);
	if (it != Windows.end()) {
		return true;
	}
	else {
		return false;
	}
}

/*Сделать окно главным*/
void SetWindowToMain(string id) {
	if (HasWindow(id)) {
		MainWindow = id;
		SetSessionInfo("MainWindow",id);
	}
	else {
		PW("Window not found! SetWindowToMain('"+id+"')","W0005");
	}
}

/*Задаёт новое название окна*/
void SetWindowTitle(string id, string title) {
	glfwSetWindowTitle(GetWindowByID(id).glfw, StringToConstChar(title));
}

/*Получить окно по айди*/
Window GetWindowByID(string id) {
	auto it = Windows.find(id);
	if (it != Windows.end()) {
		return it->second;
	}
	else {
		PF("Window not found! GetWindowByID('"+id+"')", "C0019");
		return Window();
	}
}

/*Изменить размер окна*/
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

/*Получить размер окна*/
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

/*Изменение авторазмера OpenGL*/
void SetWindowAutosize(string id, bool b) {
	Window w = GetWindowByID(id);
	w.AutoResize = b;
	Windows[id] = w;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	//	DestroyWindowGLFW("test");
	//}
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
			Windows[id] = window_;
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
		glfwDestroyWindow(window.glfw);
		P("WINDOW", "Window [" + id + "] destroyed!");
		Windows.erase(id);
	}
}