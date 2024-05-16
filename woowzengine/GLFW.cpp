#define NOMINMAX 1
#define byte win_byte_override
#include <Windows.h>

#include <iostream>
#include <map>
#include <string>
#include <functional>
//---- ������� ----
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include <glfw3native.h>
#include <glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//-----------------

#include "GLFW.h"
#include "Base.h"
#include "Easyer.h"
#include "LuaCompile.h"
#include "OpenGame.h"
#include "Files.h"
#include "Cycles.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Window.h"

#include "l_Sprite.h"
#include "Scene.h"

using namespace std;

map<string, Window> Windows;
map<GLFWwindow*, string> Windows_2;
map<string, map<string, GLuint>> Textures;
string MainWindow = "";

map<string, Scene> Scenes;

string DefaultShaderVertex = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec2 scale;

void main()
{
	vec2 NewPosition = aPos.xy * scale;
	gl_Position = vec4(NewPosition, 0, 1);
}
)";

string DefaultShaderFragment = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = color;
} 
)";

void StopGLFW() {
	glfwTerminate();
}

/*�������� �������� ����� stb_image*/
unsigned char* LoadTexture(string path, int* x, int* y, int* numchannel) {
	bool error = false;
	stbi_set_flip_vertically_on_load(true);
	if (!HasDirectory(path)) { PE("Texture ["+path+"] not found!", "E0013"); error = true;}
	unsigned char* texture = stbi_load(StringToConstChar(path), x, y, numchannel, 0);
	if (!texture) {
		PE("Texture [" + path + "] corrupted or doesn't fit", "E0014"); error = true;
	}
	if (error) {
		texture = stbi_load(StringToConstChar(GetSessionInfo("SourcePath") + "engine/error.png"), x, y, numchannel, 0);
	}
	return texture;
}

GLuint LoadSprite(string path, l_Sprite spritedata) {
	int x, y, numchan;
	unsigned char* imagedata = LoadTexture(path, &x, &y, &numchan);
	GLuint sprite;
	glGenTextures(1, &sprite);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (spritedata.Linear? GL_LINEAR : GL_NEAREST));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (spritedata.Linear ? GL_LINEAR : GL_NEAREST));
	glTexImage2D(GL_TEXTURE_2D, 0, (numchan==3? GL_RGB : GL_RGBA), x, y, 0, (numchan == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, imagedata);

	stbi_image_free(imagedata);

	return sprite;
}

void CreateBuffers(Window& window) {
	if (window.id!="") { P("OPENGL", "Create buffers for [" + window.id + "]!"); }

	glGenVertexArrays(1, &window.Arrays);
	glGenBuffers(1, &window.Buffer);

	glBindVertexArray(window.Arrays);
	glBindBuffer(GL_ARRAY_BUFFER, window.Buffer);

	GLuint VertexShader = CompileShader(DefaultShaderVertex, true);
	GLuint FragmentShader = CompileShader(DefaultShaderFragment, false);

	GLuint Shader = CompileShaderProgram(VertexShader, FragmentShader);
	string ShaderID = "default";
	map<string, map<string, int>> Uniforms = {};
	Uniforms[ShaderID]["time"] = glGetUniformLocation(Shader, "time");
	Uniforms[ShaderID]["random"] = glGetUniformLocation(Shader, "random");
	Uniforms[ShaderID]["scale"] = glGetUniformLocation(Shader, "scale");
	Uniforms[ShaderID]["color"] = glGetUniformLocation(Shader, "color");

	window.Shaders[ShaderID] = Shader;
	window.Uniforms = Uniforms;
}

void GLAPIENTRY PE_OPENGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	PF(message, "OPENGL");
}

void GLFWInstall() {
	int major, minor, revision;
	if (!glfwInit()) {
		PF("GLFW cannot be realized!","C0017");
	}
	else {
		glfwGetVersion(&major, &minor, &revision);
		P("GLFW", "GLFW Loaded! (minor-" + to_string(minor) + ",major-" + to_string(major) + ",revision-" + to_string(revision) + ")");
	}

	if (!StringToBool(GetSessionInfo("Debug"))) { glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); }
	GLFWwindow* window = glfwCreateWindow(300,300, StringToConstChar("Debug window"), NULL, NULL);

	glfwMakeContextCurrent(window);
	int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
	P("OpenGL", "OpenGL Loaded! (minor-" + to_string(GLAD_VERSION_MINOR(version)) + ",major-" + to_string(GLAD_VERSION_MAJOR(version)) + ")");
	glDebugMessageCallback(PE_OPENGL, nullptr);

	Window window_ = Window("",window);
	CreateBuffers(window_);
	Windows[""] = window_;
	Windows_2[window_.glfw] = "";
	if (!StringToBool(GetSessionInfo("Debug"))) { glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); }

	SetSessionInfo("MainWindow", "");
}

void PE_GLFW() {
	int error;
	const char* description;
	error = glfwGetError(&description);
	if (error != GLFW_NO_ERROR) {
		PE(description, "S" + to_string(error));
	}
}

/*������*/

GLuint CompileShader(string shadercode, bool ThatVertex) {
	GLuint Shader = glCreateShader(ThatVertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	const char* shadercode_ = StringToConstChar(shadercode);
	glShaderSource(Shader, 1, &shadercode_, NULL);
	glCompileShader(Shader);

	GLint compileStatus;

	glGetShaderiv(Shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		int length;
		glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &length);
		char* cMessage = new char[length];

		glGetShaderInfoLog(Shader, length, &length, cMessage);
		PE(cMessage, "SHADER");
		delete[] cMessage;
		glDeleteShader(Shader);
	}

	return Shader;
}

GLuint CompileShaderProgram(GLuint Vertex, GLuint Fragment) {
	GLuint Shader = glCreateProgram();
	glAttachShader(Shader, Vertex);
	glAttachShader(Shader, Fragment);
	glLinkProgram(Shader);
	glDeleteShader(Vertex);
	glDeleteShader(Fragment);

	GLint linkStatus;

	glGetProgramiv(Shader, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus)
	{ 
		PE("Error shader linking program!","E0012");
	}

	return Shader;
}

/*������*/

GLuint GetTexture(string window,l_Sprite sprite) {
	map<string, GLuint> Textures_;
	if (Textures.find(window) != Textures.end()) {
		Textures_ = Textures[window];
	}
	else {
		Textures_ = {};
	}

	GLuint result;
	if (Textures_.find(sprite.id) != Textures_.end()) {
		result = Textures_[sprite.id];
	}
	else {
		GLuint texture = LoadSprite(sprite.texture, sprite);
		Textures_[sprite.id] = texture;
		result = texture;
	}

	Textures[window] = Textures_;

	return result;
}

void RenderQuad(list<float> v) {
	float vert[12] = { 0 };
	int i = 0;
	for (float element : v) {
		vert[i] = element;
		i++;
	}
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vert, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_QUADS, 0, 4);
}

void UpdateShader(Window window,Color color, int width, int height, bool autosize) {
	GLuint Shader = GetFromMap(window.Shaders, "default");
	glUseProgram(Shader);
	glUniform1f(GetFromMap(GetFromMap(window.Uniforms, "default"), "time"), GetActiveTime());
	glUniform2f(GetFromMap(GetFromMap(window.Uniforms, "default"), "scale"), (window.AutoResize||autosize ? (float)1 : (float)window.StartSizeX / (float)width), (window.AutoResize||autosize ? (float)1 : (float)window.StartSizeY / (float)height));
	hash<std::string> hasher;
	glUniform1f(GetFromMap(GetFromMap(window.Uniforms, "default"), "random"), abs(sin(GetActiveTime() * hasher(window.id) / ((521673 - GetActiveTime()) + 0.5))));
	glUniform4f(GetFromMap(GetFromMap(window.Uniforms, "default"), "color"), (float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255);
}

Vector2 ScreenToWorld(Window window ,int cordx, int cordy) {
	float x,y;
	int WSX, WSY;
	glfwGetWindowSize(window.glfw, &WSX, &WSY);

	x = (2.0f * static_cast<float>(cordx) / static_cast<float>(WSX)) - 1.0f;
	y = 1.0f - (2.0f * static_cast<float>(cordy) / static_cast<float>(WSY));

	if (window.scene != "") {
		Scene scene = GetScene(window.scene);
		x = x + scene.CameraPosition.x;
		y = y + scene.CameraPosition.y;
	}

	return Vector2(x,y);
}

Vector2 ScreenToWorld(Window window,Vector2 sc) {
	if (window.scene == "") {
		PW("Window doesn't have a scene! ScreenToWorld("+window.id+",Vector2("+to_string(sc.x) + ","+to_string(sc.y) + "))", "W0006");
		return Vector2(0, 0);
	}
	int WSX, WSY;
	glfwGetWindowSize(window.glfw, &WSX, &WSY);
	float xw = static_cast<float>(WSX), yw = static_cast<float>(WSY);
	Scene scene = GetScene(window.scene);
	float x =  ((sc.x-xw)/xw+0.5) * 2 * (window.AutoResize ? 1 : (xw / 500));
	float y = -((sc.y-yw)/yw+0.5) * 2 * (window.AutoResize ? 1 : (yw / 500));

	x /= scene.CameraZoom;
	y /= scene.CameraZoom;
	x -= scene.CameraPosition.x;
	y -= scene.CameraPosition.y;
	return Vector2(x, y);
}

Vector2 WorldToScreen(Window window, Vector2 world, Scene scene, float xw, float yw) {
	if (window.scene == "") {
		PW("Window doesn't have a scene! WorldToScreen(" + window.id + ", Vector2(" + to_string(world.x) + "," + to_string(world.y) + "))", "W0007");
		return Vector2(0, 0);
	}

	if (scene.name == "") {
		scene = GetScene(window.scene);
	}
	if (xw == -1 || yw == -1) {
		int WSX, WSY;
		glfwGetWindowSize(window.glfw, &WSX, &WSY);
		xw = static_cast<float>(WSX);
		yw = static_cast<float>(WSY);
	}

	float zoom = (1 / scene.CameraZoom);
	float x = world.x + scene.CameraPosition.x/zoom;
	float y = world.y + scene.CameraPosition.y/zoom;;



	x = ((x * (window.AutoResize ? 1 : (500 / xw))) + 1) / 2;
	y = ((y * (window.AutoResize ? 1 : (500 / yw))) + 1) / 2;

	x *= xw;
	y *= yw;

	return Vector2(round(x), round(y));
}
bool PointOutside(Window window, Vector2 world, Scene scene, float xw, float yw) {
	Vector2 screen = WorldToScreen(window, world);
	if (scene.name == "") {
		scene = GetScene(window.scene);
	}
	if (xw == -1 || yw == -1) {
		int WSX, WSY;
		glfwGetWindowSize(window.glfw, &WSX, &WSY);
		xw = static_cast<float>(WSX);
		yw = static_cast<float>(WSY);
	}
	bool result = true;
	if (screen.x <= xw && screen.x >= 0 && screen.y <= yw && screen.y >= 0) {
		result = false;
	}
	return result;
}

void RenderSprite(Window window, string id, l_Sprite sprite, int width, int height,Scene scene) {
	if (sprite.id != "") {
		float Zoom = scene.CameraZoom;
		float SizeMult = (float)window.StartSizeY / (float)window.StartSizeX;
		float SizeX = sprite.size.x * SizeMult * Zoom;
		float SizeY = sprite.size.y * Zoom;
		float PosX = sprite.position.x * Zoom;
		float PosY = sprite.position.y * Zoom;

		float BLX = -SizeX + PosX;
		float BLY = -SizeY + PosY;
		float TLX = -SizeX + PosX;
		float TLY =  SizeY + PosY;
		float TRX =  SizeX + PosX;
		float TRY =  SizeY + PosY;
		float BRX =  SizeX + PosX;
		float BRY = -SizeY + PosY;
		float CENTERX = (BLX / 4) + (BRX / 4) + (TLX / 4) + (TRX / 4);
		float CENTERY = (BLY / 4) + (BRY / 4) + (TLY / 4) + (TRY / 4);

		float xw = static_cast<float>(width);
		float yw = static_cast<float>(height);

		if (sprite.DontHide || (!(
			PointOutside(window, Vector2(CENTERX,CENTERY),scene,xw,yw) && 
			PointOutside(window, Vector2(BLX, BLY)       ,scene,xw,yw) &&
			PointOutside(window, Vector2(TLX, TLY)       ,scene,xw,yw) &&
			PointOutside(window, Vector2(TRX, TRY)       ,scene,xw,yw) &&
			PointOutside(window, Vector2(BRX, BRY)       ,scene,xw,yw)
			))) {

			UpdateShader(window, sprite.color.ToCPP(), width, height, sprite.autoresize);
			GLuint texture = GetTexture(window.id, sprite);
			glBindTexture(GL_TEXTURE_2D, texture);

			float CamPosX = (sprite.movewithcamera ? 0 : scene.CameraPosition.x) * Zoom;
			float CamPosY = (sprite.movewithcamera ? 0 : scene.CameraPosition.y) * Zoom;

			RenderQuad({
				BLX + CamPosX,BLY + CamPosY, 0.0f, /*������  �����  */
				TLX + CamPosX,TLY + CamPosY, 0.0f, /*������� �����  */
				TRX + CamPosX,TRY + CamPosY, 0.0f, /*������� ������ */
				BRX + CamPosX,BRY + CamPosY, 0.0f  /*������  ������ */
			});
			
			glFlush();
		}
	}
}

void ErrorScene(string text) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0,0,0,1);
}

void Render() {
	if (Windows.size() > 0) {
		for (auto const& [id, window] : Windows) {
			if (!glfwWindowShouldClose(window.glfw)) {
					glfwMakeContextCurrent(window.glfw);
					glfwPollEvents();
					int width = 1, height = 1;
					glfwGetFramebufferSize(window.glfw, &width, &height);
					float scale = window.scale;
					glViewport(0, 0, round(width * scale), round(height * scale));
					/*----------------[���������]------------------*/
					if (window.scene != "") {
						Scene scene = GetScene(window.scene);
						if (scene.CameraZoom != 0) {
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
							glEnable(GL_BLEND);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

							if (scene.windowid != "") {
								glClearColor(scene.BackgroundColor.GetR(), scene.BackgroundColor.GetG(), scene.BackgroundColor.GetB(), 1);
							}
							else {
								glClearColor(0, 0, 0, 1);
							}
							glEnable(GL_TEXTURE_2D);
							if (scene.sprites.size() > 0) {
								for (auto const& [sid, sprite] : scene.sprites) {
									RenderSprite(window, sid, sprite, width, height, scene);
								}
							}

						}
						else {
							ErrorScene("Camera zoom cannot be equal to 0!");
						}
					}
					else {
						ErrorScene("Empty scene");
					}
					glfwSwapBuffers(window.glfw);
					/*------------[����� ���������]----------------*/
					PE_GLFW();
			}
			else {
					if (id == MainWindow) {
						Exit();
					}
					DestroyWindowGLFW(id);
				}
		}
	}
}

/*---------------------------*/

l_Sprite GetSprite(string sceneid, string id) {
	Scene scene = GetScene(sceneid);
	if (scene.sprites.find(id) == scene.sprites.end()) {
		PF("No sprite found! GetSprite('"+sceneid+"','"+id+"')","C0027");
		return l_Sprite("");
	}
	else {
		return scene.sprites[id];
	}
}

Scene GetScene(string id) {
	if (Scenes.find(id) == Scenes.end()) {
		PF("No scene found! GetScene('"+id+"')","C0026");
		return Scene("");
	}
	else {
		return Scenes[id];
	}
}

void SetSprite(l_Sprite sprite) {
	Scene scene = GetScene(sprite.sceneid);
	scene.SetSprite(sprite);
	Scenes[sprite.sceneid] = scene;
}

void CreateScene(string id) {
	P("SCENE", "Scene ["+id+"] created!");
	Scene scene(id);
	Scenes[id] = scene;
}

void SetSceneBackgroundColor(string id, Color color) {
	Scene scene = GetScene(id);
	scene.BackgroundColor = color;
	Scenes[id] = scene;
}

void SetCameraZoom(string id, float f) {
	Scene scene = GetScene(id);
	scene.CameraZoom = f;
	Scenes[id] = scene;
}

float GetCameraZoom(string id) {
	return GetScene(id).CameraZoom;
}

void CreateSprite(string id, string sceneid) {
	l_Sprite sprite(id,sceneid);
	Scene scene = GetScene(sceneid);
	scene.sprites[id] = sprite;
	Scenes[sceneid] = scene;
}

void SetSpritePosition(string sceneid, string id, l_Vector2 pos) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.position = pos;
	SetSprite(sprite);
}

void SetCameraPosition(string id, float pos, bool thatX) {
	Scene scene = GetScene(id);
	if (thatX) {
		scene.CameraPosition = Vector2(pos, scene.CameraPosition.y);
	}
	else {
		scene.CameraPosition = Vector2(scene.CameraPosition.x, pos);
	}
	Scenes[id] = scene;
}

float GetCameraPosition(string id, bool thatX) {
	Scene scene = GetScene(id);
	if (thatX) {
		return scene.CameraPosition.x;
	}
	else {
		return scene.CameraPosition.y;
	}
}

Vector2 GetMousePosition(string id) {
	Window window = GetWindowByID(id);
	double x, y;
	glfwGetCursorPos(window.glfw, &x, &y);
	return Vector2(x,y);
}

/*---------------------------*/

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

/*�������� ������� ������*/
int GetWindowPosition(string id, bool ThatX) {
	int x; int y;
	glfwGetWindowPos(GetWindowByID(id).glfw, &x, &y);
	return (ThatX ?x:y);
}

/*���������� ������� ����*/
void SetWindowPosition(string id, int pos, bool ThatX) {
	int x; int y;
	Window window = GetWindowByID(id);
	glfwGetWindowPos(window.glfw, &x, &y);
	glfwSetWindowPos(window.glfw, (ThatX?pos:x), (!ThatX ? pos:y));
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

/*�������� �����*/
void SetWindowScene(string id, string b, bool DontPrint) {
	if (!DontPrint) { P("SCENE", "Scene [" + b + "] applied to window [" + id + "]"); }
	Window w = GetWindowByID(id);
	Scene scene = GetScene(b);
	scene.windowid = id;
	Scenes[b] = scene;
	w.scene = b;
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

/*������� ������� � ����� � ����*/
void SetWindowMPEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowMousePress = f;
	Windows[id] = w;
}

/*������� ������� � ����� � ����*/
void SetWindowMREvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowMouseRelease = f;
	Windows[id] = w;
}

/*������� ������� � ����� � ����*/
void SetWindowMHEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowMouseRepeat = f;
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
	{90,"z"},{88,"x"},{67,"c"},{86,"v"},{66,"b"},{78,"n"},{77,"m"},{44,"comma"},{46,"dot"},{47,"question"},
	{346,"alt2"},{345,"ctrl2"},{269,"end"},{281,"scrolllock"},{284,"pausebreak"},{348,"applications"}
};

map<int, int> PressedKeys = {};
map<int, int> PressedMouse = {};

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
	if (Windows.size() > 1) {
		for (const auto& p : PressedKeys) {
			if (p.second > 0) {
				k[GetKeyFromID(p.first)] = p.second;
			}
		}
	}
	else {
		PE("Can't get pressed keys! There's no window for that!","E0016");
	}
	return k;
}

void MouseCallback(GLFWwindow* window_, int key_, int action, int mods) {
	Window window = GetWindowByWindow(window_);
	if (action == GLFW_PRESS) {
		if (window.WindowMousePress.valid()) {
			StartFunction(window.WindowMousePress, { key_ + 1 });
		}
		PressedMouse[key_] = 1;
	}
	if (action == GLFW_RELEASE) {
		if (window.WindowMouseRelease.valid()) {
			StartFunction(window.WindowMouseRelease, { key_ + 1 });
		}
		PressedMouse[key_] = 0;
	}
	if (action == GLFW_REPEAT) {
		if (window.WindowMouseRepeat.valid()) {
			StartFunction(window.WindowMouseRepeat, { key_ + 1 });
		}
		PressedMouse[key_] = PressedMouse[key_] + 1;
	}
}

void KeyCallback(GLFWwindow* window_, int key_, int scancode, int action, int mods) {
	Window window = GetWindowByWindow(window_);
	if (action == GLFW_PRESS) {
		if (window.WindowKeyPress.valid()) {
			StartFunction(window.WindowKeyPress, { GetKeyFromID(key_), key_ });
		}
		PressedKeys[key_] = 1;
	}
	if (action == GLFW_RELEASE) {
		if (window.WindowKeyRelease.valid()) {
			StartFunction(window.WindowKeyRelease, { GetKeyFromID(key_), key_ });
		}
		PressedKeys[key_] = 0;
	}
	if (action == GLFW_REPEAT) {
		if (window.WindowKeyRepeat.valid()) {
			StartFunction(window.WindowKeyRepeat, { GetKeyFromID(key_), key_ });
		}
		PressedKeys[key_] = PressedKeys[key_] + 1;
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
			glfwMakeContextCurrent(window);
			Window window_ = Window(id,window);
			CreateBuffers(window_);
			window_.StartSizeX = sizex;
			window_.StartSizeY = sizey;
			glfwSetKeyCallback(window, KeyCallback);
			glfwSetMouseButtonCallback(window, MouseCallback);

			Windows[id] = window_;
			Windows_2[window] = id;

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
			StartFunction(window.WindowClosed, {});
		}
		glfwMakeContextCurrent(window.glfw);
		for (auto it = window.Shaders.begin(); it != window.Shaders.end(); ++it) {
			glDeleteProgram(it->second);
		}
		glDeleteVertexArrays(1, &window.Arrays);
		glDeleteBuffers(1, &window.Buffer);
		glfwDestroyWindow(window.glfw);
		P("WINDOW", "Window [" + id + "] destroyed!");
		Windows_2.erase(window.glfw);
		Windows.erase(id);
	}
}