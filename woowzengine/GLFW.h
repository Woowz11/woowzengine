#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <sol/sol.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <list>
#include "Vertex.h"
#include "Window.h"
#include "Scene.h"
#include "Color.h"
#include "RenderElement.h"
#include "l_Sprite.h"
#include "Vector2.h"
#pragma once
using namespace std; 

void UpdateShader(Window window,Color color,int width, int height, bool autosize = false);
void RenderQuad(Window window, list<float> v);
void SetWindowTitle(string id, string title);
void SetWindowSize(string id, bool thatY, int size);
int GetWindowSize(string id, bool thatY);
void StopGLFW();
void GLFWInstall();
Vector2 ScreenToWorld(Window window, int cordx, int cordy);
void RenderSprite(Window window, string id, l_Sprite sprite, int width, int height);
void Render();
bool HasWindow(string id);
Window GetWindowByID(string id);
void SetWindowToMain(string id);
void SetWindowScale(string id, float scale = 1);
void PE_GLFW();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window_, int key_, int action, int mods);
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
void SetWindowMPEvent(string id, sol::function f);
void SetWindowMREvent(string id, sol::function f);
void SetWindowMHEvent(string id, sol::function f);
void SetWindowScene(string id, string b, bool DontPrint = false);
map<string, int> GetPressedKeys();
GLuint CompileShader(string shadercode, bool ThatVertex);
GLuint CompileShaderProgram(GLuint Vertex, GLuint Fragment);
GLuint GetTexture(string window, l_Sprite sprite);
unsigned char* LoadTexture(string path, int* x, int* y, int* numchannel);
GLuint LoadSprite(string path, l_Sprite spritedata);
void CreateBuffers(Window& window);
void GLAPIENTRY PE_OPENGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
int GetWindowPosition(string id, bool ThatX);
void SetWindowPosition(string id, int pos, bool ThatX);

void SetCameraZoom(string id, float f);
float GetCameraZoom(string id);
void SetSprite(l_Sprite sprite);
l_Sprite GetSprite(string sceneid, string id);
Scene GetScene(string id);
void CreateScene(string id);
void RemoveScene(string id);
void CreateSprite(string id, string sceneid);
void SetSceneBackgroundColor(string id, Color color);
void SetSpritePosition(string sceneid, string id, l_Vector2 pos);
void SetCameraPosition(string id, float pos, bool thatX);
float GetCameraPosition(string id, bool thatX);
Vector2 GetMousePosition(string id);
Vector2 ScreenToWorld(Window window,Vector2 sc);
Vector2 WorldToScreen(Window window, Vector2 world);
bool PointOutside(Window window, Vector2 world);