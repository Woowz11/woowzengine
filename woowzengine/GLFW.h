#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <sol/sol.hpp>
#include <glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Vertex.h"
#include "Window.h"
#include "Scene.h"
#include "RenderElement.h"
#pragma once
using namespace std; 

void RenderObjectTriangles(Vertex v1, Vertex v2, Vertex v3);
void SetWindowTitle(string id, string title);
void SetWindowSize(string id, bool thatY, int size);
int GetWindowSize(string id, bool thatY);
void StopGLFW();
void GLFWInstall();
void RenderElement_(RenderElement e);
void Render();
bool HasWindow(string id);
Window GetWindowByID(string id);
void SetWindowToMain(string id);
void SetWindowScale(string id, float scale = 1);
void PE_GLFW(int error, const char* desc);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLEW(Window);
Window CreateWindowGLFW(string id, int sizex = 640, int sizey = 480, string title = "New Window");
void DestroyWindowGLFW(string id);
int GetWindowSize(string id, bool thatY);
void SetWindowAutosize(string id, bool b);
void SetWindowResizable(string id, bool b);
void SetWindowClosedEvent(string id, sol::function f);
string GetIDByWindow(GLFWwindow* window);
Window GetWindowByWindow(GLFWwindow* window);
string GetKeyFromID(int i);
void SetWindowKPEvent(string id, sol::function f);
void SetWindowKREvent(string id, sol::function f);
void SetWindowKHEvent(string id, sol::function f);
void SetWindowScene(string id, Scene b, bool = false);
map<string, int> GetPressedKeys();
GLuint CompileShader(string shadercode, bool ThatVertex);
GLuint CompileShaderProgram(GLuint Vertex, GLuint Fragment);