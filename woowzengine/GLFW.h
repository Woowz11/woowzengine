#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Vertex.h"
#include "Window.h"
#pragma once
using namespace std; 

void RenderObjectTriangles(Vertex v1, Vertex v2, Vertex v3);
void SetWindowTitle(string id, string title);
void SetWindowSize(string id, bool thatY, int size);
int GetWindowSize(string id, bool thatY);
void StopGLFW();
void GLFWInstall();
void Render();
bool HasWindow(string id);
Window GetWindowByID(string id);
void SetWindowToMain(string id);
void PE_GLFW(int error, const char* desc);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
Window CreateWindowGLFW(string id, int sizex = 640, int sizey = 480, string title = "New Window");
void DestroyWindowGLFW(string id);
int GetWindowSize(string id, bool thatY);
void SetWindowAutosize(string id, bool b);