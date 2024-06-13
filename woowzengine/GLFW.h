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
#include "Texture.h"
#include "Color.h"
#include "RenderElement.h"
#include "l_Sprite.h"
#include "l_Text.h"
#include "Vector2.h"
#include "l_Font.h"
#pragma once
using namespace std; 

void UpdateShader(Window window, string shaderid, l_Color color,int width, int height, bool autosize = false, float z = 0, float charpos = 0, float stringlenght = 0);
void RenderQuad(list<float> v_uv);
void SetWindowTitle(string id, string title);
void SetWindowSize(string id, bool thatY, int size);
int GetWindowSize(string id, bool thatY);
void StopGLFW();
void ErrorScene(string text);
void GLFWInstall();
Window CreateShaders(Window window, string shaderpath);
Vector2 ScreenToWorld(Window window, int cordx, int cordy);
void RenderSprite(Window window, string id, l_Sprite sprite, int width, int height, Scene scene);
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
GLuint GetTexture(l_Sprite sprite);
unsigned char* LoadTexture(string path, int* x, int* y, int* numchannel);
GLuint LoadSprite(string path, l_Sprite spritedata, bool savecolors = false);
GLuint LoadSprite(string path, Texture texture, bool savecolors = false);
GLuint LoadSprite_(Texture texture, unsigned char* colors, int x, int y, int numchan, bool savecolors = false, bool that_stbi = false);
void CreateBuffers(Window& window);
void GLAPIENTRY PE_OPENGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
int GetWindowPosition(string id, bool ThatX);
void SetWindowPosition(string id, int pos, bool ThatX);
void SetCameraZoom(string id, float f);
float GetCameraZoom(string id);
void SetSprite(l_Sprite sprite);
l_Sprite GetSprite(string sceneid, string id, bool = false);
Scene GetScene(string id);
void CreateScene(string id);
void CreateSprite(string id, string sceneid);
void SetSceneBackgroundColor(string id, Color color);
void SetSpritePosition(string sceneid, string id, l_Vector2 pos);
void SetSpriteCenter(string sceneid, string id, l_Vector2 pos);
void SetCameraPosition(string id, float pos, bool thatX);
float GetCameraPosition(string id, bool thatX);
Vector2 GetMousePosition(string id);
Vector2 ScreenToWorld(Window window,Vector2 sc);
Vector2 WorldToScreen(Window window, float x_ = 0, float y_ = 0, Scene scene = Scene(""), float xw = -1, float yw = -1);
Vector2 WorldToScreen(Window window, Vector2 world, Scene scene = Scene(""), float xw = -1, float yw = -1);
bool PointOutside(Window window, float x = 0, float y = 0, Scene scene = Scene(""), float xw = -1, float yw = -1, float Offset = 0);
bool PointOutside(Window window, Vector2 world, Scene scene = Scene(""), float xw = -1, float yw = -1, float offset = 0);
bool PointOutside_(l_Vector2 vec, string windowid);
void SetSpriteSize(string sceneid, string id, l_Vector2 size);
void SetSpriteCorner(string sceneid, string id, l_Vector2 pos, bool left, bool top);
void SetSpriteCornerUV(string sceneid, string id, l_Vector2 pos, bool left, bool top);
void SetWindowTransparency(string id, int alpha);
int GetWindowTransparency(string id);
void SetSpriteColor(string sceneid, string id, l_Color color);
l_Color GetSpriteColor(string sceneid, string id);
void SetSpriteLayer(string sceneid, string id, float zindex);
float GetSpriteLayer(string sceneid, string id);
vector<string> GetSpritesOnScene(string sceneid);
string GetTextureID(Texture sprite);
void UpdateLayers(string sceneid);
bool HasSprite(string sceneid, string id);
void SetSpriteTexture(string sceneid, string id, string texture);
GLuint GetTextureT(Texture texture);
void GetTextureTT(Texture texture, bool savecolors = false);
void GetTextureTTT(Texture texture, int sizex, int sizey, unsigned char* colors_result, int numchan, bool savecolors);
void SetSpriteSizeByTexture(string sceneid, string id, float sizeextra = 1);
l_Vector2 GetTextureSize(string texture);
l_Vector2 GetSpriteSize(string sceneid, string id);
l_Vector2 GetSpritePosition(string sceneid, string id);
l_Vector2 GetSpriteCenter(string sceneid, string id);
void SetSpriteMirror(string sceneid, string id, bool ThatX, bool b);
bool GetSpriteMirror(string sceneid, string id, bool ThatX);
void SetSpriteOrientation(string sceneid, string id, float deg);
l_Vector2 RotatePoint(l_Vector2 point, l_Vector2 center, float angle);
float DegToRad(float deg);
float RadToDeg(float rad);
float lerp(float, float, float);
float GetSpriteRotation(string sceneid, string id);
vector<string> GetWindows();
string GetSceneByWindow(string sceneid);
string GetWindowByScene(string windowid);
Texture GetTextureByID(string id);
void CreateTexture(string id, string path, bool savecolors = false);
void SetSpriteVisible(string sceneid, string id, bool visible);
bool GetSpriteVisible(string sceneid, string id);
bool HasTexture(string id);
void SetTextureBlur(string id, bool blur);
vector<l_Color> GetTextureColors(string textureid);
vector<string> split(string str, char delimiter);
void CreateTextureByArray(string id, int sizex, int sizey, vector<l_Color> colors, bool savecolors = false);
void WriteImage(string path, int sizex, int sizey, vector<l_Color> colors);
void WriteImage(string path, int sizex, int sizey, unsigned char* colors_result, int numchan, bool rotate = false, bool flip = false);
vector<l_Color> StringToVectorColors(string col, unordered_map<char, l_Color> colorkeys);
void WriteImage(string path, int sizex, int sizey, string colors_, unordered_map<char, l_Color> colorkeys);
unsigned char* VectorColorsToChars(vector<l_Color> colors, int sizex, int sizey, int* numchan, bool reverse_ = true, bool flipx = false);
void SetSpriteShader(string sceneid, string id, string shader);
void SetSpriteHeight(string sceneid, string id, float height);
string GetSpriteShader(string sceneid, string id);
float GetSpriteHeight(string sceneid, string id);
bool HasText(string sceneid, string id);
void CreateText(string id, string sceneid, string text);
void RenderText(Window window, string id, l_Text text, int width, int height, Scene scene);
void GLFWTest();
void CreateFont(string id, string path);
bool GenerateFont_(l_Font font, string sceneid);
void UpdateTexturesWindowCreated();
l_Text GetText(string sceneid, string id);
void SetText(l_Text text);
void SetTextPosition(string sceneid, string id, l_Vector2 pos);
void SetTextFont(string sceneid, string id, string font);
void SetTextColor(string sceneid, string id, l_Color col);
void SetTextText(string sceneid, string id, string text_);
void SetTextHeight(string sceneid, string id, float height);
void ShowCursor_(string id, bool b);

void SetTextMono(string sceneid, string id, bool b);