#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <map>
#include <string>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>/*Окна*/
#include <glfw3native.h>
//---- Графика ----
#include <glew.h>
#include <wglew.h>
#include <glm.hpp>
//-----------------
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GLFW.h"
#include "Base.h"
#include "Easyer.h"
#include "LuaCompile.h"
#include "OpenGame.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Window.h"

using namespace std;

map<string, Window> Windows;
map<GLFWwindow*, string> Windows_2;
string MainWindow = "";

GLFWwindow* DebugWindow;

string VertexDefaultShader = R"(
    #version 330
    in vec3 pos;
    in vec2 texCoord;

    out vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(pos, 1.0);
        TexCoord = texCoord;
    }
)";

string FragmentDefaultShader = R"(
    #version 330 core
	out vec4 FragColor;
  
	in vec3 ourColor;
	in vec2 TexCoord;

	uniform sampler2D ourTexture;

	void main()
	{
	    FragColor = texture(ourTexture, TexCoord);
	}
)";

GLuint DefaultShader;

void StopGLFW() {
	glDeleteProgram(DefaultShader);
	glfwTerminate();
}

/*Загрузка текстуры через stb_image*/
unsigned char* LoadTexture(string path, int* x, int* y, int* numchannel) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texture = stbi_load(StringToConstChar(path), x, y, numchannel, 0);
	if (!texture) {
		PE("ERROR TEXTURE", "");
	}
	return texture;
}

GLuint LoadSprite(string path) {
	int x, y, numchan;
	unsigned char* imagedata = LoadTexture(path, &x, &y, &numchan);
	GLuint sprite;
	glGenTextures(1, &sprite);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);

	stbi_image_free(imagedata);

	return sprite;
}

GLuint testsprite;
GLuint textureLocation;
void GLEW(Window window) {
	SetSessionInfo("GLEWwindow", "true");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		const char* errString = reinterpret_cast<const char*>(glewGetErrorString(err));
		PF(errString, "GLEW");
	}
	P("GLEW", "GLEW installed on ["+window.id+"]!");
	glfwSetErrorCallback(PE_GLFW);

	GLuint vShader = CompileShader(VertexDefaultShader, true);
	GLuint fShader = CompileShader(FragmentDefaultShader, false);
	DefaultShader = CompileShaderProgram(vShader, fShader);
	textureLocation = glGetUniformLocation(DefaultShader, "ourTexture");

	testsprite = LoadSprite("F:/woowzengine/example_game/game/engine/error.png");
}

void GLFWInstall() {
	int major, minor, revision;
	if (!glfwInit()) {
		PF("GLFW cannot be realized!","C0017");
	}
	else {
		glfwGetVersion(&major, &minor, &revision);
	}
	P("GLFW", "GLFW Installed! (minor - " + to_string(minor) + ",major-" + to_string(major) + ",revision-" + to_string(revision) + ")");

	SetSessionInfo("MainWindow", "");
}

void PE_GLFW(int error, const char* desc) {
	string desc_ = desc;
	PE(desc_,"S"+to_string(error));
}

/*Рендер*/

GLuint CompileShader(string shadercode, bool ThatVertex) {
	GLuint Shader = glCreateShader(ThatVertex? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
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

/*Методы*/

void RenderElement_(RenderElement e) {
	string type = e.type;

	//if (type == "triangle") {
	//	RenderObjectTriangles(GetFromList(e.Vertexs, 0), GetFromList(e.Vertexs, 1), GetFromList(e.Vertexs, 2));
	//}

	glFlush();
}

void Render() {
	if (Windows.size() > 0) {
		for (auto const& [id, window] : Windows) {
			if (!glfwWindowShouldClose(window.glfw)) {

					glfwMakeContextCurrent(window.glfw);

					int width, height;
					if (window.AutoResize) {
						glfwGetFramebufferSize(window.glfw, &width, &height);
					}
					else {
						width = window.StartSizeX;
						height = window.StartSizeY;
					}
					glViewport(0, 0, round(width * window.scale), round(height * window.scale));

					/*----------------[Рисование]------------------*/

					glUseProgram(DefaultShader);

					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					if (window.scene.windowid != "") {
						glClearColor(window.scene.BackgroundColor.GetR(), window.scene.BackgroundColor.GetG(), window.scene.BackgroundColor.GetB(), 1);
					}
					else {
						glClearColor(0, 0, 0, 1);
					}
				
					if (window.WindowGLEW) {

					}

					//glActiveTexture(GL_TEXTURE0);
					//glBindTexture(GL_TEXTURE_2D, testsprite);
					//glUniform1i(glGetUniformLocation(testsprite, "myTexture"), 0);

					/*glColor3f(1.0f, 1.0f, 1.0f);
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(centerX - halfSquareWidth, centerY - halfSquareHeight);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(centerX + halfSquareWidth, centerY - halfSquareHeight);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(centerX + halfSquareWidth, centerY + halfSquareHeight);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(centerX - halfSquareWidth, centerY + halfSquareHeight);
					glEnd();*/
					
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, testsprite);
					glUniform1i(textureLocation, 0);

					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(-1,-1);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(-1,1);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(1,1);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(1,-0.5);
					glEnd();


					glfwSwapBuffers(window.glfw);
					/*------------[Конец рисования]----------------*/
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
		PF("Window not found! GetWindowByID('" + id + "')", "C0019");
		return Window();
	}
}

/*Получить айди по GLFWwindow*/
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

/*Получить окно по GLFWwindow*/
Window GetWindowByWindow(GLFWwindow* window) {
	return GetWindowByID(GetIDByWindow(window));
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

/*Добавить сцену*/
void SetWindowScene(string id, Scene b, bool DontPrint) {
	if (!DontPrint) { P("SCENE", "Scene [" + b.name + "] applied to window [" + id + "]"); }
	Window w = GetWindowByID(id);
	b.windowid = id;
	w.scene = b;
	Windows[id] = w;
}

/*Изменение маштаба*/
void SetWindowScale(string id, float scale) {
	if (scale <= 0) { PE("Scale cannot be <= 0! SetWindowScale('"+id+"',"+to_string(scale) + ")", "E0009"); }
	else {
		Window w = GetWindowByID(id);
		w.scale = scale;
		Windows[id] = w;
	}
}

/*Запретить менять размер окна или нет*/
void SetWindowResizable(string id, bool b) {
	glfwSetWindowAttrib(GetWindowByID(id).glfw,GLFW_RESIZABLE,b);
}

/*Ивент при закрытии окна*/
void SetWindowClosedEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowClosed = f;
	Windows[id] = w;
}

/*Нажатие клавиши в окне*/
void SetWindowKPEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowKeyPress = f;
	Windows[id] = w;
}

/*Отжатие клавиши в окне*/
void SetWindowKREvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowKeyRelease = f;
	Windows[id] = w;
}

/*Зажатие клавиши в окне*/
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
	{90,"z"},{88,"x"},{67,"c"},{86,"v"},{66,"b"},{78,"n"},{77,"m"},{44,"comma"},{46,"dot"},{47,"question"},
	{346,"alt2"},{345,"ctrl2"},{269,"end"},{281,"scrolllock"},{284,"pausebreak"},{348,"applications"}
};

map<int, bool> PressedKeys = {};

/*Получить клавишу по айди*/
string GetKeyFromID(int i) {
	string key = "key_"+to_string(i);
	if (Keys.find(i) != Keys.end()) {
		return Keys.find(i)->second;
	}
	return key;
}

/*Получить в данный момент зажатые клавиши*/
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
			glfwMakeContextCurrent(window);
			Window window_ = Window(id,window);
			window_.StartSizeX = sizex;
			window_.StartSizeY = sizey;
			glfwSetKeyCallback(window, KeyCallback);

			P("WINDOW", "Window [" + id + "] created!");

			if (StringToBool(GetSessionInfo("GLEWwindow")) == false) {
				window_.WindowGLEW = true;
				GLEW(window_);
			}

			Windows[id] = window_;
			Windows_2[window] = id;

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