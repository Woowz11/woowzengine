#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#pragma warning(disable : 4267)

#define NOMINMAX 1
#define byte win_byte_override
#include <Windows.h>

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
//---- Графика ----
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include <glfw3native.h>
#include <glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"
//-----------------

#include "GLFW.h"
#include "Base.h"
#include "Easyer.h"
#include "WindowsElements.h"
#include "LuaCompile.h"
#include "OpenGame.h"
#include "Files.h"
#include "Cycles.h"
#include "Texture.h"
#include "WConst.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Window.h"

#include "l_Sprite.h"
#include "l_Text.h"
#include "l_Font.h"
#include "Scene.h"

using namespace std;

map<string, Window> Windows;
map<GLFWwindow*, string> Windows_2;
unordered_map<string, GLuint> Textures;
unordered_map<string, Texture> Textures_;
unordered_map<string, unordered_map<string, string>> Textures_Info;
string MainWindow = "";

unordered_map <string, vector <l_Sprite>> SceneSprites;
unordered_map <string, vector <l_Text>> SceneTexts;

unordered_map<string, Scene> Scenes;

unordered_map<string, l_Font> Fonts;
unordered_map <string, vector<string>> SceneFonts;
unordered_map<string, unordered_map<string, GLuint>> Chars;

string NowWindow;

void UpdateTexturesWindowCreated() {
	for (auto pair : Windows) {
		string sceneid = pair.second.scene;
		if (sceneid != "") {
			glfwMakeContextCurrent(pair.second.glfw);
			Scene scene = Scenes[sceneid];
			for (auto pair2 : Fonts) {
				GenerateFont_(pair2.second, scene.name);
			}
		}
	}
}

unordered_map<int, string> Chars_IDS = {
	{32," "},{48,"0"},{49,"1"},{50,"2"},{51,"3"},{52,"4"},{53,"5"},{54,"6"},{55,"7"},{56,"8"},{57,"9"},
	{33,"!"},{63,"?"},{45,"-"},{43,"+"},{61,"="},{95,"_"},{35,"#"},{36,"№"},{64,"@"},{126,"~"},{96,"`"},{39,"'"},
	{34,"\""},{58,":"},{59,";"},{46,"."},{44,","},{37,"%"},{94,"^"},{38,"&"},{42,"*"},{40,"("},{41,")"},
	{91,"["},{93,"]"},{123,"{"},{125,"}"},{60,"<"},{62,">"},{92,"\\"},{124,"|"},{47,"/"},{65,"A"},{97,"a"},
	{66,"B"},{98,"b"},{67,"C"},{99,"c"},{68,"D"},{100,"d"},{69,"E"},{101,"e"},{70,"F"},{102,"f"},{71,"G"},
	{103,"g"},{72,"H"},{104,"h"},{73,"I"},{105,"i"},{74,"J"},{106,"j"},{75,"K"},{107,"k"},{76,"L"},{108,"l"},
	{77,"M"},{109,"m"},{78,"N"},{110,"n"},{79,"O"},{111,"o"},{80,"P"},{112,"p"},{81,"Q"},{113,"q"},{82,"R"},{114,"r"},
	{83,"S"},{115,"s"},{84,"T"},{116,"t"},{85,"U"},{117,"u"},{86,"V"},{118,"v"},{87,"W"},{119,"w"},{88,"X"},{120,"x"},
	{89,"Y"},{121,"y"},{90,"Z"},{122,"z"}
};

unordered_map<int, string> Chars_Pos = {
	{0, " "}, {1, "0"}, {2, "1"}, {3, "2"}, {4, "3"}, {5, "4"}, {6, "5"}, {7, "6"}, {8, "7"}, {9, "8"}, {10, "9"},
	{11, "!"}, {12, "?"}, {13, "-"}, {14, "+"}, {15, "="}, {16, "_"}, {17, "#"}, {18, "№"}, {19, "@"}, {20, "%"}, {21, "&"}, {22, "*"},
	{23, "("}, {24, ")"}, {25, "["}, {26, "]"}, {27, "{"}, {28, "}"}, {29, "<"}, {30, ">"}, {31, "^"},
	{32, "."}, {33, ","}, {34, ":"}, {35, ";"}, {36, "'"}, {37, "\""}, {38, "`"}, {39, "~"}, {40, "/"}, {41, "|"}, {42, "\\"},
	{48, "A"}, {49, "a"}, {50, "B"}, {51, "b"}, {52, "C"}, {53, "c"}, {54, "D"}, {55, "d"}, {56, "E"}, {57, "e"}, {58, "F"}, {59, "f"}, {60, "G"}, {61, "g"}, {62, "H"},
	{63, "h"}, {64, "I"}, {65, "i"}, {66, "J"}, {67, "j"}, {68, "K"}, {69, "k"}, {70, "L"}, {71, "l"}, {72, "M"}, {73, "m"}, {74, "N"}, {75, "n"}, {76, "O"}, {77, "o"},
	{78, "P"}, {79, "p"}, {80, "Q"}, {81, "q"}, {82, "R"}, {83, "r"}, {84, "S"}, {85, "s"}, {86, "T"}, {87, "t"}, {88, "U"}, {89, "u"}, {90, "V"}, {91, "v"}, {92, "W"},
	{93, "w"}, {94, "X"}, {95, "x"}, {96, "Y"}, {97, "y"}, {98, "Z"}, {99, "z"},

	{255, "error"}
};

void StopGLFW() {
	glfwTerminate();
}

bool GenerateFont_(l_Font font, string sceneid) {
	vector<string> fonts = SceneFonts[sceneid];
	if (find(fonts.begin(), fonts.end(), font.id) == fonts.end()) {
		int x, y, numchan;
		unsigned char* fonttexture = LoadTexture(font.image, &x, &y, &numchan);
		if (x != y) {
			PE("", "");
			delete[] fonttexture;
			return false;
		}
		if (x % 16 != 0) {
			PE("", "");
			delete[] fonttexture;
			return false;
		}

		if (numchan <= 3) {
			numchan = 4;
			unsigned char* destination = new unsigned char[x*y* numchan];
			for (int i = 0; i < x * y; i++) {
				destination[i * numchan] = fonttexture[i * 3];
				destination[i * numchan + 1] = fonttexture[i * 3 + 1];
				destination[i * numchan + 2] = fonttexture[i * 3 + 2];
				destination[i * numchan + 3] = min(fonttexture[i * 3]+fonttexture[i * 3 + 1]+ fonttexture[i * 3 + 2],255);
			}
			delete[] fonttexture;
			fonttexture = destination;
		}

		int size = x / 16;
		unordered_map<string, GLuint> chars;
		int nx = 0;
		int ny = 15;
		for (int c = 0; c < 256; c++) {
				int cx = nx * size;
				int cy = ny * size;
				int cw = size;
				int ch = size;

				nx++;
				if (nx > 15) {
					nx = 0;
					ny--;
				}

			if (Chars_Pos.find(c) != Chars_Pos.end()) {
				unsigned char* chartexture = new unsigned char[cw * ch * numchan];
				for (int i = 0; i < ch; i++) {
					for (int j = 0; j < cw; j++) {
						for (int k = 0; k < numchan; k++) {
							if (numchan > 3) {
								chartexture[(i * cw + j) * numchan + k] = fonttexture[((i + cy) * x + j + cx) * numchan + k];
							}
							else {
								chartexture[(i * cw + j) * numchan + k] = fonttexture[((i + cy) * x + j + cx) * numchan + k];
							}
						}
					}
				}

				string charid = "Font (" + font.id + ") " + Chars_Pos[c];
				Texture texture = Texture("");
				texture.sceneid = sceneid;
				texture.id = charid;
				texture.TextureID = GetTextureID(texture);
				Textures_[charid] = texture;

				GLuint texture_ = LoadSprite_(texture, chartexture, cw, ch, numchan);
				Textures[texture.TextureID] = texture_;


				chars[Chars_Pos[c]] = texture_;
			}
		}
		Chars[font.id] = chars;

		fonts.push_back(font.id);
		SceneFonts[sceneid] = fonts;
		return true;
	}
	return false;
}

void CreateFont(string id, string path) {
	l_Font font(id,path);
	if (GenerateFont_(font,"")) {
		Fonts[id] = font;
		P("FONT", "Font [" + id + "] created!");
		UpdateTexturesWindowCreated();
	}
	else {
		PE("Such a font is already exsist! CreateFont('"+id+"','"+path+"')","E0038");
	}
}

/*Загрузка текстуры через stb_image*/
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

string GetTextureID(Texture texture) {
	string result = texture.path;
	if (result == "") {
		result = texture.id;
	}

	if (texture.Linear) {
		result = result + " [LINEAR]";
	}

	if (texture.sceneid != "") {
		result = texture.sceneid + " " + result;
	}

	return result;
}

GLuint LoadSprite(string path, l_Sprite spritedata, bool savecolors) {
	int x, y, numchan;
	unsigned char* colors = LoadTexture(path, &x, &y, &numchan);
	return LoadSprite_(GetTextureByID(spritedata.texture), colors, x, y, numchan, savecolors, true);
}

GLuint LoadSprite(string path, Texture texture, bool savecolors) {
	int x, y, numchan;
	unsigned char* colors = LoadTexture(path, &x, &y, &numchan);
	return LoadSprite_(texture, colors, x, y, numchan, savecolors, true);
}

GLuint LoadSprite_(Texture texture, unsigned char* colors, int x, int y, int numchan, bool savecolors, bool that_stbi) {
	unsigned char* imagedata = colors;
	GLuint sprite;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &sprite);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (texture.Linear? GL_LINEAR : GL_NEAREST));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (texture.Linear ? GL_LINEAR : GL_NEAREST));
	GLint imagechannels = (numchan == 4 ? GL_RGBA : (numchan == 1 ? GL_RED : (numchan == 2 ? GL_RG : GL_RGB)));
	glTexImage2D(GL_TEXTURE_2D, 0, imagechannels, x, y, 0, imagechannels, GL_UNSIGNED_BYTE, imagedata);

	string textureid = texture.id;
	Textures_Info[textureid]["x"] = to_string(x);
	Textures_Info[textureid]["y"] = to_string(y);
	Textures_Info[textureid]["hasalpha"] = (numchan >= 4 ? "true" : "false");

	if (savecolors) {
		int size = x * y * numchan;

		string ColorsR = "";
		string ColorsG = "";
		string ColorsB = "";
		string ColorsA = "";
		
		string ColorsR_ = "";
		string ColorsG_ = "";
		string ColorsB_ = "";
		string ColorsA_ = "";
		int colorid = 1;
		int j = 0;
		for (int i = 0; i < (size); i++) {
			switch (colorid) {
			case 1:
				ColorsR_ = ColorsR_ + to_string(imagedata[i]) + ",";
				break;
			case 2:
				ColorsG_ = ColorsG_ + to_string(imagedata[i]) + ",";
				break;
			case 3:
				ColorsB_ = ColorsB_ + to_string(imagedata[i]) + ",";
				break;
			case 4:
				ColorsA_ = ColorsA_ + to_string(imagedata[i]) + ",";
				break;
			}
			colorid++;
			if (colorid > numchan) {
				colorid = 1;
				j++;
			}
			if (j >= x) {
				ColorsR = ColorsR_ + ColorsR;
				ColorsG = ColorsG_ + ColorsG;
				ColorsB = ColorsB_ + ColorsB;
				ColorsA = ColorsA_ + ColorsA;
				ColorsR_ = "";
				ColorsG_ = "";
				ColorsB_ = "";
				ColorsA_ = "";
				j = 0;
			}
		}

		ColorsR.pop_back();
		ColorsG.pop_back();
		ColorsB.pop_back();
		if (StringToBool(Textures_Info[textureid]["hasalpha"])) { ColorsA.pop_back(); };
		Textures_Info[textureid]["r"] = ColorsR;
		Textures_Info[textureid]["g"] = ColorsG;
		Textures_Info[textureid]["b"] = ColorsB;
		Textures_Info[textureid]["a"] = ColorsA;
	}
	else {
		Textures_Info[textureid]["r"] = "";
		Textures_Info[textureid]["g"] = "";
		Textures_Info[textureid]["b"] = "";
		Textures_Info[textureid]["a"] = "";
	}

	if (that_stbi) { stbi_image_free(imagedata); }

	return sprite;
}

unordered_map<string,bool> Shaders;
Window CreateShaders(Window window, string shaderpath) {
	string ShaderID = ReplaceString(GetFileName(shaderpath), "." + GetFileType(shaderpath), "");
	if (window.id == "") {
		if (!Shaders[ShaderID]) {
			P("SHADER", "Shader [" + ShaderID + "] compiled!");
			Shaders[ShaderID] = true;
		}
	}
	else {
		string Folder = ReplaceString(shaderpath, GetFileName(shaderpath), "");
		if (!window.Shaders[ShaderID]) {
			bool hasvertex = HasDirectory(Folder + ShaderID + ".v");
			bool hasfragment = HasDirectory(Folder + ShaderID + ".f");

			string vertex = "";
			string fragment = "";

			if (hasvertex) {
				vertex = ReadFile(Folder + ShaderID + ".v");
			}
			else {
				vertex = ReadFile(Folder + "default.v");
			}

			if (hasfragment) {
				fragment = ReadFile(Folder + ShaderID + ".f");
			}
			else {
				fragment = ReadFile(Folder + "default.f");
			}

			GLuint VertexShader = CompileShader(vertex, true);
			GLuint FragmentShader = CompileShader(fragment, false);

			GLuint Shader = CompileShaderProgram(VertexShader, FragmentShader);
			unordered_map<string, int> U = {};
			U["sprite"] = glGetUniformLocation(Shader, "sprite");
			U["time"] = glGetUniformLocation(Shader, "time");
			U["random"] = glGetUniformLocation(Shader, "random");
			U["scale"] = glGetUniformLocation(Shader, "scale");
			U["color"] = glGetUniformLocation(Shader, "color");
			U["height"] = glGetUniformLocation(Shader, "height");
			U["charpos"] = glGetUniformLocation(Shader, "charpos");
			U["stringlen"] = glGetUniformLocation(Shader, "stringlen");
			U["zoom"] = glGetUniformLocation(Shader, "zoom");

			window.Shaders[ShaderID] = Shader;
			window.Uniforms[ShaderID] = U;

			if (window.id == "") { P("SHADER", "Shader [" + ShaderID + "] compiled!"); }
		}
	}
	return window;
}

void GLFWTest() {

}

void CreateBuffers(Window& window) {
	if (window.id!="") { P("OPENGL", "Create buffers for [" + window.id + "]!"); }

	glGenVertexArrays(1, &window.Arrays);
	glGenBuffers(1, &window.Buffer);

	glBindVertexArray(window.Arrays);
	glBindBuffer(GL_ARRAY_BUFFER, window.Buffer);

	for (auto path : GetFilesFromDirectory(GetSessionInfo("SourcePath") + "/engine/shaders")) {
		string typ = Lowercase(GetFileType(path.second));
		if (typ == "v" || typ == "f") {
			window = CreateShaders(window, path.second);
		}
	}
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
	if(SupportedWindowsVersion()){
		glDebugMessageCallback(PE_OPENGL, nullptr);
	}
	else {
		P("OpenGL","OpenGL errors will not be displayed on <= Windows 7!");
	}

	Window window_ = Window("", window);
	CreateBuffers(window_);
	Windows[""] = window_;
	Windows_2[window_.glfw] = "";
	if (!StringToBool(GetSessionInfo("Debug"))) { glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); }

	SetSessionInfo("MainWindow", "");

	CreateTexture("default", GetSessionInfo("SourcePath") + "engine/default.png");
	CreateTexture("error", GetSessionInfo("SourcePath") + "engine/error.png");

	CreateFont("default", GetSessionInfo("SourcePath") + "engine/fonts/default.png");
}

void PE_GLFW() {
	int error;
	const char* description;
	error = glfwGetError(&description);
	if (error != GLFW_NO_ERROR) {
		PE(description, "S" + to_string(error));
	}
}

/*Рендер*/

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

/*Методы*/

GLuint GetTexture(l_Sprite sprite) {
	unordered_map<string, GLuint> Textures_ = Textures;
	Texture t = GetTextureByID(sprite.texture);
	string textureid = GetTextureID(t);

	GLuint result;
	if (Textures_.find(textureid) != Textures_.end()) {
		result = Textures_[textureid];
	}
	else {
		if (t.ThatTextureGenerated) {
			GLuint texture = LoadSprite_(t,t.imagedata,t.sizex,t.sizey,t.numchan);
			Textures_[textureid] = texture;
			result = texture;
		}
		else {
			GLuint texture = LoadSprite(t.path, sprite);
			Textures_[textureid] = texture;
			result = texture;
		}
	}

	Textures = Textures_;

	return result;
}

void RenderQuad(list<float> v_uv) {
	const int size = 20;
	float vert[size] = { 0 };
	int i = 0;
	for (float element : v_uv) {
		vert[i] = element;
		i++;
	}
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vert, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_QUADS, 0, 4);
}

void UpdateShader(Window window, string shaderid, l_Color color, int width, int height, bool autosize, float z, float charpos, float stringlenght, float zoom) {
	GLuint Shader = window.Shaders[shaderid];
	glUseProgram(Shader);
	if (window.Uniforms[shaderid]["sprite"] != -1) {
		glUniform1i(window.Uniforms[shaderid]["sprite"], 0);
	}
	if (window.Uniforms[shaderid]["time"] != -1) {
		glUniform1f(window.Uniforms[shaderid]["time"], GetActiveTime());
	}
	if (window.Uniforms[shaderid]["scale"] != -1) {
		glUniform2f(window.Uniforms[shaderid]["scale"], (window.AutoResize || autosize ? (float)1 : (float)window.StartSizeX / (float)width), (window.AutoResize || autosize ? (float)1 : (float)window.StartSizeY / (float)height));
	}
	if (window.Uniforms[shaderid]["random"] != -1) {
		hash<std::string> hasher;
		glUniform1f(window.Uniforms[shaderid]["random"], abs(sin(GetActiveTime() * hasher(window.id) / ((521673 - GetActiveTime()) + 0.5))));
	}
	if (window.Uniforms[shaderid]["color"] != -1) {
		glUniform4f(window.Uniforms[shaderid]["color"], (float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255);
	}
	if (window.Uniforms[shaderid]["height"] != -1) {
		glUniform1f(window.Uniforms[shaderid]["height"], pow(2, -z));
	}
	if (window.Uniforms[shaderid]["charpos"] != -1) {
		glUniform1f(window.Uniforms[shaderid]["charpos"], charpos);
	}
	if (window.Uniforms[shaderid]["stringlen"] != -1) {
		glUniform1f(window.Uniforms[shaderid]["stringlen"], stringlenght);
	}
	if (window.Uniforms[shaderid]["zoom"] != -1) {
		glUniform1f(window.Uniforms[shaderid]["zoom"], zoom);
	}
}

Vector2 ScreenToWorld(Window window ,int cordx, int cordy) {
	float x,y;
	int WSX, WSY;
	glfwGetWindowSize(window.glfw, &WSX, &WSY);

	x = (2.0f * static_cast<float>(cordx) / static_cast<float>(WSX)) - 1.0f;
	y = 1.0f - (2.0f * static_cast<float>(cordy) / static_cast<float>(WSY));

	if (window.scene != "") {
		Scene scene = GetScene(window.scene);
		x = x - scene.CameraPosition.x;
		y = y - scene.CameraPosition.y;
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
	float x =  ((sc.x-xw)/xw+0.5) * 2 * (window.AutoResize ? 1 : (xw / window.StartSizeX));
	float y = -((sc.y-yw)/yw+0.5) * 2 * (window.AutoResize ? 1 : (yw / window.StartSizeY));

	x /= scene.CameraZoom;
	y /= scene.CameraZoom;
	x += scene.CameraPosition.x;
	y += scene.CameraPosition.y;
	return Vector2(x, y);
}

Vector2 WorldToScreen(Window window, float x_, float y_, Scene scene, float xw, float yw) {
	if (window.scene == "") {
		PW("Window doesn't have a scene! WorldToScreen(" + window.id + ", Vector2(" + to_string(x_) + "," + to_string(y_) + "))", "W0007");
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
	float SizeMult = (float)window.StartSizeY / (float)window.StartSizeX;
	float x = x_ - scene.CameraPosition.x * SizeMult / zoom;
	float y = y_ - scene.CameraPosition.y / zoom;



	x = ((x * (window.AutoResize ? 1 : (window.StartSizeX / xw))) + 1) / 2;
	y = ((y * (window.AutoResize ? 1 : (window.StartSizeY / yw))) + 1) / 2;

	x *= xw;
	y *= yw;

	return Vector2(round(x), round(y));
}

Vector2 WorldToScreen(Window window, Vector2 world, Scene scene, float xw, float yw) {
	return WorldToScreen(window, world.x, world.y, scene, xw, yw);
}

bool PointOutside(Window window, float x, float y, Scene scene, float xw, float yw, float Offset) {
	Vector2 screen = WorldToScreen(window, x, y);
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
	if (screen.x <= (xw+Offset) && screen.x >= (-Offset) && screen.y <= (yw+Offset) && screen.y >= (-Offset)) {
		result = false;
	}
	return result;
}

bool PointOutside(Window window, Vector2 world, Scene scene, float xw, float yw, float Offset) {
	return PointOutside(window, world.x, world.y, scene, xw, yw, Offset);
}

bool PointOutside_(l_Vector2 vec, string windowid) {
	Window window = GetWindowByID(windowid);
	return PointOutside(window, vec.ToCPP(), GetScene(window.scene));
}

float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

float PI = 3.1415926535;
float DegToRad(float deg) {
	return deg * (PI / 180);
}

float RadToDeg(float rad) {
	return rad * (180 / PI);
}

l_Vector2 RotatePoint(l_Vector2 point, l_Vector2 center, float angle_) {
	float angle = DegToRad(angle_);
	float x = center.x + (point.x - center.x) * cos(angle) - (point.y - center.y) * sin(angle);
	float y = center.y + (point.x - center.x) * sin(angle) + (point.y - center.y) * cos(angle);
	return l_Vector2(x, y);
}

void RenderSprite(Window window, string id, l_Sprite sprite, int width, int height,Scene scene) {
	if (sprite.id != "") {
		float Zoom = scene.CameraZoom;
		float Angle = sprite.angle;
		float SizeX = sprite.size.x * Zoom;
		float SizeY = sprite.size.y * Zoom;
		float PosX = sprite.position.x * Zoom;
		float PosY = sprite.position.y * Zoom;

		float CamPosX = (sprite.movewithcamera ? 0 : scene.CameraPosition.x) * Zoom;
		float CamPosY = (sprite.movewithcamera ? 0 : scene.CameraPosition.y) * Zoom;

		float xw = float(width);
		float yw = float(height);

		float BLX = -SizeX + PosX + (sprite.LB.x * Zoom);
		float BLY = -SizeY + PosY + (sprite.LB.y * Zoom);
		float TLX = -SizeX + PosX + (sprite.LT.x * Zoom);
		float TLY = SizeY + PosY + (sprite.LT.y * Zoom);
		float TRX = SizeX + PosX + (sprite.RT.x * Zoom);
		float TRY = SizeY + PosY + (sprite.RT.y * Zoom);
		float BRX = SizeX + PosX + (sprite.RB.x * Zoom);
		float BRY = -SizeY + PosY + (sprite.RB.y * Zoom);

		float CENTERX = (BLX + TLX + TRX + BRX) / 4 + sprite.origin.x;
		float CENTERY = (BLY + TLY + TRY + BRY) / 4 + sprite.origin.y;
		float HeightScale = (pow(2, -sprite.Height));
		float OutNumber = 400 * Zoom * HeightScale * max(sprite.size.x+(abs(sprite.LB.x) + abs(sprite.LT.x) + abs(sprite.RB.x) + abs(sprite.RT.x)), sprite.size.y+(abs(sprite.LB.y) + abs(sprite.LT.y) + abs(sprite.RB.y) + abs(sprite.RT.y)));
		float HeightOffsetX = (CamPosX - CENTERX) * HeightScale;
		float HeightOffsetY = (CamPosY - CENTERY) * HeightScale;
		bool Out = PointOutside(window,CENTERX+HeightOffsetX,CENTERY+HeightOffsetY,scene,xw,yw,FloatToInt(OutNumber));
		if (!Out){

			l_Vector2 RotCenter = l_Vector2(CENTERX, CENTERY);

			l_Vector2 LB = RotatePoint(l_Vector2(BLX, BLY), RotCenter, Angle);
			l_Vector2 LT = RotatePoint(l_Vector2(TLX, TLY), RotCenter, Angle);
			l_Vector2 RB = RotatePoint(l_Vector2(BRX, BRY), RotCenter, Angle);
			l_Vector2 RT = RotatePoint(l_Vector2(TRX, TRY), RotCenter, Angle);

			GLuint texture = GetTexture(sprite);
			glBindTexture(GL_TEXTURE_2D, texture);
			UpdateShader(window, sprite.shader, sprite.color, width, height, sprite.autoresize, sprite.Height, 0, 0, Zoom);

			float flipx = 1;
			if (sprite.FlipX) {
				flipx = -1;
			}
			float flipy = 1;
			if (sprite.FlipY) {
				flipy = -1;
			}

			RenderQuad({
			/* [ Corners                    ]   [ UV                                    ] */
				LB.x-CamPosX,LB.y-CamPosY, 0,    sprite.UVLB.x*flipx,sprite.UVLB.y*flipy,
				LT.x-CamPosX,LT.y-CamPosY, 0,    sprite.UVLT.x*flipx,sprite.UVLT.y*flipy,
				RT.x-CamPosX,RT.y-CamPosY, 0,    sprite.UVRT.x*flipx,sprite.UVRT.y*flipy,
				RB.x-CamPosX,RB.y-CamPosY, 0,    sprite.UVRB.x*flipx,sprite.UVRB.y*flipy
			});
		}
			
		glFlush();
	}
}

void RenderText(Window window, string id, l_Text text, int width, int height, Scene scene) {
	if (text.id != "" && text.font != "") {
		l_Font font = Fonts[text.font];

		float x = text.position.x;
		float y = text.position.y;

		float charscalesymbol = 1;
		float mirrory = 1;
		float mirrorx = 1;

		float Zoom = scene.CameraZoom;

		int length = text.text.length();
		for (int i = 0; i < length; i++) {
			char c = text.text[i];

			float bearingx = 0;
			float bearingy = 0;
			float sizex = 1;
			float sizey = 1;
			float otstyp = 0;
			float scale = text.scale * charscalesymbol;

			float xpos = (x + bearingx * scale) * Zoom;
			float ypos = (y - (sizey - bearingy) * scale) * Zoom;
			float w = sizex * scale * Zoom;
			float h = sizey * scale * Zoom;

			float CamPosX = scene.CameraPosition.x * Zoom;
			float CamPosY = scene.CameraPosition.y * Zoom;

			float xw = float(width);
			float yw = float(height);

			float BLX = xpos;
			float BLY = ypos;
			float TLX = xpos;
			float TLY = ypos + h;
			float TRX = xpos + w;
			float TRY = ypos + h;
			float BRX = xpos + w;
			float BRY = ypos;

			l_Vector2 LB = l_Vector2(BLX, BLY);
			l_Vector2 LT = l_Vector2(TLX, TLY);
			l_Vector2 RB = l_Vector2(BRX, BRY);
			l_Vector2 RT = l_Vector2(TRX, TRY);
			l_Vector2 CENTER = l_Vector2(xpos+w/2,ypos+h/2);

			float HeightScale = (pow(2, -text.height));
			float OutNumber = 400 * Zoom * HeightScale;
			bool Out = PointOutside(window, CENTER.x, CENTER.y, scene, xw, yw, FloatToInt(OutNumber));

			bool dontrender = false;
			if (c == 0 || (c >= 1 && c <= 8) || c == 10 || Out) {
				dontrender = true;
			}
			if (!dontrender) {
				string charid = Chars_IDS[c];
				if (charid == "") {
					charid = "error";
				}
				GLuint texture = Chars[text.font][charid];
				glBindTexture(GL_TEXTURE_2D, texture);
				UpdateShader(window, text.shader, text.color, width, height, false, text.height, i, length, Zoom);

				RenderQuad({
					LB.x - CamPosX,LB.y - CamPosY, 0,    0,      0,
					LT.x - CamPosX,LT.y - CamPosY, 0,    0,      mirrory,
					RT.x - CamPosX,RT.y - CamPosY, 0,    mirrorx,mirrory,
					RB.x - CamPosX,RB.y - CamPosY, 0,    mirrorx,0
				});
			}

			if (!((c >= 1 && c <= 8) || c == 10)) { x += (sizex + otstyp * Zoom) * scale; }
			if (c == 10) {
				y -= (sizey + otstyp) * scale;
				x = text.position.x;
			}
			if (c == 1) {
				mirrory = mirrory * -1;
			}
			if (c == 8) {
				mirrorx = mirrorx * -1;
			}
			if (c == 2) {
				y += 0.2  * scale;
			}
			if (c == 3) {
				y -= 0.2 * scale;
			}
			if (c == 4) {
				x += 0.2 * scale;
			}
			if (c == 5) {
				x -= 0.2 * scale;
			}
			if (c == 6) {
				charscalesymbol *= 1.1;
			}
			if (c == 7) {
				charscalesymbol /= 1.1;
			}
		}

		glFlush();
	}
}

void ErrorScene(string text) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0,0,0,1);
}

void Render() {
	if (Windows.size() > 0) {
		for (auto const& [id, window] : Windows) {
			NowWindow = id;
			if (!glfwWindowShouldClose(window.glfw)) {
				glfwMakeContextCurrent(window.glfw);
				glfwPollEvents();
				int width = 1, height = 1;
				glfwGetFramebufferSize(window.glfw, &width, &height);
				float scale = window.scale;
				glViewport(0, 0, round(width * scale), round(height * scale));
				/*----------------[Рисование]------------------*/
				if (window.scene != "") {
					Scene scene = GetScene(window.scene);
					if (scene.CameraZoom != 0) {
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glEnable(GL_BLEND);
						glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

						if (scene.windowid != "") {
							float alpha = scene.BackgroundColor.GetA();
							glClearColor(scene.BackgroundColor.GetR() * alpha, scene.BackgroundColor.GetG() * alpha, scene.BackgroundColor.GetB() * alpha, alpha);
						}
						else {
							glClearColor(0, 0, 0, 1);
						}


						glEnable(GL_TEXTURE_2D);
						vector<l_Sprite> sprites = SceneSprites[window.scene];
						if (sprites.size() > 0) {

							for (const auto& sprite : sprites) {
								if (sprite.Visible) {
									RenderSprite(window, sprite.id, sprite, width, height, scene);
								}
							}
						}

						vector<l_Text> texts = SceneTexts[window.scene];
						if (texts.size() > 0) {

							for (const auto& text : texts) {
								if (text.Visible) {
									RenderText(window, text.id, text, width, height, scene);
								}
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
				/*------------[Конец рисования]----------------*/
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

void UpdateLayers(string sceneid) {
	vector<l_Sprite> sprites = SceneSprites[sceneid];
	for (auto& sprite : sprites) {
		sprite.zindex_code = sprite.zindex + (float(sprite.cout)/10000) + (sprite.Height*100000);
	}
	std::sort(sprites.begin(), sprites.end(), [](const l_Sprite& a, const l_Sprite& b) {
		return a.zindex_code < b.zindex_code;
	});
	SceneSprites[sceneid].clear();
	SceneSprites[sceneid] = sprites;
}

bool HasSprite(string sceneid, string id) {
	Scene scene = GetScene(sceneid);
	vector<l_Sprite> sprites = SceneSprites[sceneid];
	auto it = find_if(sprites.begin(), sprites.end(), [&](const l_Sprite& sprite_) {
		return sprite_.id == id;
		});

	if (it != sprites.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool HasText(string sceneid, string id) {
	Scene scene = GetScene(sceneid);
	vector<l_Text> texts = SceneTexts[sceneid];
	auto it = find_if(texts.begin(), texts.end(), [&](const l_Text& text_) {
		return text_.id == id;
		});

	if (it != texts.end()) {
		return true;
	}
	else {
		return false;
	}
}

GLuint GetTextureT(Texture texture) {
	string id = GetTextureID(texture);
	if (Textures.find(id) != Textures.end()) {
		return Textures[id];
	}
	else {
		l_Sprite sprite = GetSprite(texture.sceneid, texture.spriteid, true);
		return GetTexture(sprite);
	}
}

void GetTextureTT(Texture texture, bool savecolors) {
	string id = GetTextureID(texture);
	if (Textures_.find(id) == Textures_.end()) {
		unordered_map<string, GLuint> Textures_ = Textures;
		GLuint texture_ = LoadSprite(texture.path, texture, savecolors);
		Textures_[id] = texture_;
		Textures = Textures_;

	}
}

unsigned char* VectorColorsToChars(vector<l_Color> colors, int sizex, int sizey, int* numchan, bool reverse_, bool flipx) {
	*numchan = 3;
	for (const auto c : colors) {
		if (c.a != 255) {
			*numchan = 4;
		}
	}
	int size = colors.size();
	if (size < (sizex * sizey)) {
		bool invert = false;
		int j = 0;
		int i = 0;
		for (int i = 0; i < (sizex * sizey); i++) {
			if (i >= size) {
				l_Color color = (invert ? l_Color(0, 0, 0) : ErrorColor);
				if (i % 2 == 0) {
					color = (invert ? ErrorColor : l_Color(0, 0, 0));
				}
				colors.push_back(color);
			}

			j++;
			if (j >= sizex) {
				j = 0;
				invert = !invert;
			}
		}
	}
	if (flipx) {
		vector<l_Color> colors_(sizex * sizey);

		int x_ = 0;
		int y_ = 0;
		for (int i_ = 0; i_ < (sizex * sizey); i_++) {
			colors_[(sizex-1)-x_ + (y_ * (sizex))] = colors[i_];

			x_++;
			if (x_ > (sizex-1)) {
				x_ = 0;
				y_++;
			}
		}

		colors = colors_;
	}
	if (reverse_) { reverse(colors.begin(), colors.end()); }
	unsigned char* colors_result = new unsigned char[sizex * sizey * *numchan];
	int i = 0;
	int j = 0;
	int j_ = 0;
	for (int i = 0; i < (sizex * sizey * *numchan); i++) {
		if (*numchan == 4) {
			switch (j) {
			case 0:
				colors_result[i] = colors[j_].r;
				break;
			case 1:
				colors_result[i] = colors[j_].g;
				break;
			case 2:
				colors_result[i] = colors[j_].b;
				break;
			case 3:
				colors_result[i] = colors[j_].a;
				break;
			}
		}
		else {
			switch (j) {
			case 0:
				colors_result[i] = colors[j_].r;
				break;
			case 1:
				colors_result[i] = colors[j_].g;
				break;
			case 2:
				colors_result[i] = colors[j_].b;
				break;
			}
		}

		j++;
		if (j >= *numchan) {
			j = 0;
			j_++;
		}
	}
	return colors_result;
}

void GetTextureTTT(Texture texture, int sizex, int sizey, unsigned char* colors_result, int numchan, bool savecolors) {
	string id = GetTextureID(texture);
	if (Textures_.find(id) != Textures_.end()) {
		unordered_map<string, GLuint> Textures_ = Textures;
		GLuint texture_ = LoadSprite_(texture, colors_result, sizex, sizey, numchan, savecolors);
		Textures_[id] = texture_;
		Textures = Textures_;

	}
}

bool HasTexture(string id) {
	if (Textures_.find(id) != Textures_.end()) {
		return true;
	}
	else {
		return false;
	}
}

Texture GetTextureByID(string id) {
	if (Textures_.find(id) != Textures_.end()) {
		return Textures_[id];
	}
	else {
		PE("No texture found! GetTextureByID('"+id+"')","E0026");
		return Textures_["error"];
	}
}

l_Sprite GetSprite(string sceneid, string id, bool textureerror) {
	Scene scene = GetScene(sceneid);
	vector<l_Sprite> sprites = SceneSprites[sceneid];
	auto it = find_if(sprites.begin(), sprites.end(), [&](const l_Sprite& sprite_) {
		return sprite_.id == id;
	});
	
	if (it != sprites.end()) {
		return *it;
	}
	else {
		if (textureerror) {
			PF("The texture was not loaded into the sprite! GetSprite('" + sceneid + "','" + id + "')", "C0026");
		}
		else {
			PE("No sprite found! GetSprite('" + sceneid + "','" + id + "')", "E0022");
		}
		return l_Sprite("");
	}
}

l_Text GetText(string sceneid, string id) {
	Scene scene = GetScene(sceneid);
	vector<l_Text> texts = SceneTexts[sceneid];
	auto it = find_if(texts.begin(), texts.end(), [&](const l_Text& text_) {
		return text_.id == id;
		});

	if (it != texts.end()) {
		return *it;
	}
	else {
		PE("No text found! GetText('" + sceneid + "','" + id + "')", "E0037");
		return l_Text("","");
	}
}

Scene GetScene(string id) {
	if (Scenes.find(id) == Scenes.end()) {
		PE("No scene found! GetScene('"+id+"')","E0021");
		return Scene("");
	}
	else {
		return Scenes[id];
	}
}

void SetSprite(l_Sprite sprite) {
	Scene scene = GetScene(sprite.sceneid);
	vector<l_Sprite> sprites = SceneSprites[sprite.sceneid];
	auto it = find_if(sprites.begin(), sprites.end(), [&](const l_Sprite& sprite_) {
		return sprite_.id == sprite.id;
		});
	*it = sprite;
	SceneSprites[sprite.sceneid].clear();
	SceneSprites[sprite.sceneid] = sprites;
}

void SetText(l_Text text) {
	Scene scene = GetScene(text.sceneid);
	vector<l_Text> texts = SceneTexts[text.sceneid];
	auto it = find_if(texts.begin(), texts.end(), [&](const l_Text& text_) {
		return text_.id == text.id;
		});
	*it = text;
	SceneTexts[text.sceneid].clear();
	SceneTexts[text.sceneid] = texts;
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

void CreateText(string id, string sceneid, string text) {
	if (!HasText(sceneid, id)) {
		l_Text result(id, sceneid,text);
		Scene scene = GetScene(sceneid);
		SceneTexts[sceneid].push_back(result);
	}
	else {
		PE("Such a text is already on the scene! CreateText('" + id + "','" + sceneid + "','"+text+"')", "E0036");
	}
}

void SetTextPosition(string sceneid, string id, l_Vector2 pos) {
	l_Text text = GetText(sceneid, id);
	text.position = pos;
	SetText(text);
}

void SetTextText(string sceneid, string id, string text_) {
	l_Text text = GetText(sceneid, id);
	text.text = text_;
	SetText(text);
}

void SetTextHeight(string sceneid, string id, float height) {
	l_Text text = GetText(sceneid, id);
	text.height = height;
	SetText(text);
}

void SetTextColor(string sceneid, string id, l_Color col) {
	l_Text text = GetText(sceneid, id);
	text.color = col;
	SetText(text);
}

void SetTextFont(string sceneid, string id, string font) {
	l_Text text = GetText(sceneid, id);
	text.font = font;
	SetText(text);
}

int spritescount = 0;
void CreateSprite(string id, string sceneid) {
	if (!HasSprite(sceneid, id)) {
		l_Sprite sprite(id, sceneid);
		sprite.cout = spritescount;
		spritescount++;
		Scene scene = GetScene(sceneid);
		SceneSprites[sceneid].push_back(sprite);
		UpdateLayers(sceneid);
		GetTextureTT(GetTextureByID(sprite.texture));
	}
	else {
		PE("Such a sprite is already on the scene! CreateSprite('"+id+"','"+sceneid+"')","E0023");
	}
}

void SetSpritePosition(string sceneid, string id, l_Vector2 pos) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.position = pos;
	SetSprite(sprite);
}

void SetSpriteCenter(string sceneid, string id, l_Vector2 pos) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.origin = pos;
	SetSprite(sprite);
}

void SetSpriteSize(string sceneid, string id, l_Vector2 size) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.size = size;
	SetSprite(sprite);
}

void SetSpriteOrientation(string sceneid, string id, float deg) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.angle = deg;
	SetSprite(sprite);
}

l_Vector2 GetSpriteSize(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.size;
}

l_Vector2 GetSpriteCenter(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.origin;
}

float GetSpriteRotation(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.angle;
}

l_Vector2 GetSpritePosition(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.position;
}

void SetSpriteCorner(string sceneid, string id, l_Vector2 pos, bool left, bool top) {
	l_Sprite sprite = GetSprite(sceneid, id);
	if (left) {
		if (top) {
			sprite.LT = pos;
		}
		else {
			sprite.LB = pos;
		}
	}
	else {
		if (top) {
			sprite.RT = pos;
		}
		else {
			sprite.RB = pos;
		}
	}
	SetSprite(sprite);
}

void SetSpriteCornerUV(string sceneid, string id, l_Vector2 pos, bool left, bool top) {
	l_Sprite sprite = GetSprite(sceneid, id);
	if (left) {
		if (top) {
			sprite.UVLT = pos;
		}
		else {
			sprite.UVLB = pos;
		}
	}
	else {
		if (top) {
			sprite.UVRT = pos;
		}
		else {
			sprite.UVRB = pos;
		}
	}
	SetSprite(sprite);
}

void CreateTexture(string id, string path, bool savecolors) {
	if (!HasTexture(id)) {
		Texture texture = Texture(path);
		texture.id = id;
		texture.TextureID = GetTextureID(texture);
		Textures_[id] = texture;
		GetTextureTT(texture,savecolors);
	}
	else {
		PE("Such a texture is already exsist! CreateTexture('"+id+"','"+path+"')","E0027");
	}
}

void CreateTextureByArray(string id, int sizex, int sizey, vector<l_Color> colors, bool savecolors) {
	if (!HasTexture(id)) {
		int numchan;
		unsigned char* c = VectorColorsToChars(colors, sizex, sizey, &numchan, true, true);   
		Texture texture = Texture("");
		texture.imagedata = c;
		texture.sizex = sizex;
		texture.sizey = sizey;
		texture.numchan = numchan;
		texture.ThatTextureGenerated = true;
		texture.id = id;
		texture.TextureID = GetTextureID(texture);
		Textures_[id] = texture;
		GetTextureTTT(texture, sizex,sizey,c,numchan, savecolors);
	}
	else {
		PE("Such a texture is already exsist! CreateTextureByArray('" + id + "',"+to_string(sizex) + ","+to_string(sizey) + ",vector<l_Color>)", "E0028");
	}
}

vector<l_Color> StringToVectorColors(string col, unordered_map<char, l_Color> colorkeys) {
	vector<l_Color> result;
	for (size_t i = 0; i < col.length(); i++) {
		char c = col[i];
		if (colorkeys.find(c)!=colorkeys.end()) {
			result.push_back(colorkeys[c]);
		}
		else {
			result.push_back(ErrorColor);
		}
	}
	return result;
}

void WriteImage(string path, int sizex, int sizey, string colors_, unordered_map<char,l_Color> colorkeys) {
	int numchan;
	unsigned char* colors_result = VectorColorsToChars(StringToVectorColors(colors_, colorkeys), sizex, sizey, &numchan, false,false);

	WriteImage(path,sizex,sizey,colors_result,numchan,false);
}
void WriteImage(string path, int sizex, int sizey, vector<l_Color> colors) {
	int numchan;
	unsigned char* colors_result = VectorColorsToChars(colors, sizex, sizey, &numchan);

	WriteImage(path,sizex,sizey,colors_result,numchan,true,true);
}
void WriteImage(string path, int sizex, int sizey, unsigned char* colors_result, int numchan, bool rotate, bool flip) {
	if (rotate) {
		unsigned char* rotated_colors_result = new unsigned char[sizex * sizey * numchan];

		for (int y = 0; y < sizey; y++) {
			for (int x = 0; x < sizex; x++) {
				int rotatedY = sizey - 1 - y;
				int rotatedIndex = (rotatedY * sizex + x) * numchan;
				int originalIndex = (y * sizex + x) * numchan;

				for (int c = 0; c < numchan; c++) {
					rotated_colors_result[rotatedIndex + c] = colors_result[originalIndex + c];
				}
			}
		}
		delete[] colors_result;
		colors_result = rotated_colors_result;
	}
	if (flip) {
		unsigned char* rotated_colors_result = new unsigned char[sizex * sizey * numchan];

		int x_ = 0;
		int y_ = 0;
		for (int y = 0; y < sizey; y++) {
			for (int x = 0; x < sizex; x++) {
				for (int c = 0; c < numchan; c++) {
					rotated_colors_result[((sizex-1) - x_ + (y_ * (sizex))) * numchan + c] = colors_result[(y * sizex + x) * numchan + c];
				}

				x_++;
				if (x_ > (sizex - 1)) {
					x_ = 0;
					y_++;
				}
			}
		}

		delete[] colors_result;
		colors_result = rotated_colors_result;
	}

	string filetype = Lowercase(GetFileType(path));

	int result;

	if (filetype == "png") {
		result = stbi_write_png(StringToConstChar(path), sizex, sizey, numchan, colors_result, sizex * numchan);
	}
	else if (filetype == "jpg" || filetype == "jpeg") {
		result = stbi_write_jpg(StringToConstChar(path), sizex, sizey, numchan, colors_result, sizex * numchan);
	}
	else if (filetype == "bmp") {
		result = stbi_write_bmp(StringToConstChar(path), sizex, sizey, numchan, colors_result);
	}
	else if (filetype == "tga") {
		result = stbi_write_tga(StringToConstChar(path), sizex, sizey, numchan, colors_result);
	}
	else if (filetype == "hdr") {
		float* colors_hdr = new float[sizex * sizey * numchan];
		for (int i = 0; i < (sizex * sizey * numchan); i++) {
			colors_hdr[i] = float(colors_result[i]) / 255;
		}
		result = stbi_write_hdr(StringToConstChar(path), sizex, sizey, numchan, colors_hdr);
		delete[] colors_hdr;
	}
	else {
		PE("Unknown image format. WriteImage('"+path+"',"+to_string(sizex) + ","+to_string(sizey) + ")", "E0029");
		result = stbi_write_png(StringToConstChar(path), sizex, sizey, numchan, colors_result, sizex * numchan);
	}

	if (result == 0) {
		PE("Failed to write image data. WriteImage('" + path + "'," + to_string(sizex) + "," + to_string(sizey) + ")", "E0030");
	}
	delete[] colors_result;
}

void SetTextureBlur(string id, bool blur) {
	Texture texture = GetTextureByID(id);
	texture.SetBlur(blur);
	Textures_[id] = texture;
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

void SetSpriteColor(string sceneid, string id, l_Color color) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.color = color;
	SetSprite(sprite);
}

l_Color GetSpriteColor(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.color;
}

void SetSpriteTexture(string sceneid, string id, string texture) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.texture = texture;
	Texture texture_ = GetTextureByID(texture);
	texture_.SetID(sceneid, id);
	Textures_[texture] = texture_;
	SetSprite(sprite);
}

void SetSpriteVisible(string sceneid, string id, bool visible) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.Visible = visible;
	SetSprite(sprite);
}

bool GetSpriteVisible(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.Visible;
}

l_Vector2 GetTextureSize(string texture) {
	Texture t = GetTextureByID(texture);
	string id = texture;
	int x = StringToInt(Textures_Info[id]["x"]);
	int y = StringToInt(Textures_Info[id]["y"]);
	return l_Vector2(x,y);
}

void SetSpriteSizeByTexture(string sceneid, string id, float se) {
	l_Sprite sprite = GetSprite(sceneid, id);
	l_Vector2 size = GetTextureSize(sprite.texture);
	float div = 32 * se;
	SetSpriteSize(sceneid, id, l_Vector2(size.x/div,size.y/div));
}

void SetSpriteLayer(string sceneid, string id, float zindex) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.zindex = zindex;
	SetSprite(sprite);
	UpdateLayers(sceneid);
}

void SetSpriteMirror(string sceneid, string id, bool ThatX, bool b) {
	l_Sprite sprite = GetSprite(sceneid, id);
	if (ThatX) {
		sprite.FlipX = b;
	}
	else {
		sprite.FlipY = b;
	}
	SetSprite(sprite);
}

void SetSpriteShader(string sceneid, string id, string shader) {
	if (Shaders[shader]) {
		l_Sprite sprite = GetSprite(sceneid, id);
		sprite.shader = shader;
		SetSprite(sprite);
	}
	else {
		PE("Shader not found! SetSpriteShader('" + sceneid + "','" + id + "','" + shader + "')", "E0031");
	}
}

void SetSpriteHeight(string sceneid, string id, float height) {
	l_Sprite sprite = GetSprite(sceneid, id);
	sprite.Height = height;
	SetSprite(sprite);
}

string GetSpriteShader(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.shader;
}

float GetSpriteHeight(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.Height;
}

bool GetSpriteMirror(string sceneid, string id, bool ThatX) {
	l_Sprite sprite = GetSprite(sceneid, id);
	if (ThatX) {
		return sprite.FlipX;
	}
	else {
		return sprite.FlipY;
	}
}

float GetSpriteLayer(string sceneid, string id) {
	l_Sprite sprite = GetSprite(sceneid, id);
	return sprite.zindex;
}

vector<string> GetSpritesOnScene(string sceneid) {
	Scene scene = GetScene(sceneid);

	map<float, string> newlist;
	for (auto const& sprite : SceneSprites[sceneid]) {
		newlist[sprite.zindex_code] = sprite.id;
	}

	std::vector<float> keys;
	for (const auto& pair : newlist) {
		keys.push_back(pair.first);
	}
	std::sort(keys.begin(), keys.end());

	vector<string> sortedList;
	for (const auto& key : keys) {
		sortedList.push_back(newlist[key]);
	}

	return sortedList;
}

vector<string> split(string str_, char delimiter) {
	std::vector<std::string> res;
	char* arr = StringToCharArray(str_);
	int i = 0;

	string s;
	while (arr[i] != '\0') {
		if (arr[i] != delimiter) {
			s += arr[i];
		}
		else {
			res.push_back(s);
			s.clear();
		}
		i++;
	}
	res.push_back(s);
	return res;
}

vector<l_Color> GetTextureColors(string textureid) {
	Texture texture = GetTextureByID(textureid);
	string id = textureid;
	int size = StringToInt(Textures_Info[id]["x"]) * StringToInt(Textures_Info[id]["y"]);
	string rs = Textures_Info[id]["r"];
	string gs = Textures_Info[id]["g"];
	string bs = Textures_Info[id]["b"];
	string as = Textures_Info[id]["a"];
	if (rs!="") {
		vector<l_Color> result;

		if (!StringToBool(Textures_Info[id]["hasalpha"])) {
			int i_ = 0;
			for (int i_ = 0; i_ < size; i_++) {
				as = as + "255,";
			}
			as.pop_back();
		}
		vector<string> rs_ = split(ReplaceString(rs, " ", ""), ',');
		vector<string> gs_ = split(ReplaceString(gs, " ", ""), ',');
		vector<string> bs_ = split(ReplaceString(bs, " ", ""), ',');
		vector<string> as_ = split(ReplaceString(as, " ", ""), ',');
		int j = 0;
		for (int j = 0; j < size; j++) {
			result.push_back(l_Color(StringToInt(rs_[j]), StringToInt(gs_[j]), StringToInt(bs_[j]), StringToInt(as_[j])));
		}
		return result;
	}
	else {
		PW("The texture has no colors saved, it is not possible to get a colors. GetTextureColors('"+textureid+"')", "W0009");
		vector<l_Color> result;
		result.push_back(ErrorColor);
		return result;
	}
}

vector<string> GetWindows() {
	vector<string> result;
	for (const auto& pair : Windows) {
		if (pair.first != "") {
			result.push_back(pair.first);
		}
	}

	return result;
}

string GetSceneByWindow(string windowid) {
	Window window = GetWindowByID(windowid);
	if (window.scene == "") {
		PE("No scene found in the window! GetSceneByWindow('"+windowid+"')","E0024");
		return "";
	}
	else {
		return window.scene;
	}
}

string GetWindowByScene(string sceneid) {
	Scene scene = GetScene(sceneid);
	if (scene.windowid == "") {
		PE("No window found in the scene! GetWindowByScene('" + sceneid + "')", "E0025");
		return "";
	}
	else {
		return scene.windowid;
	}
}

/*---------------------------*/

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

/*Получить позицию экрана*/
int GetWindowPosition(string id, bool ThatX) {
	int x; int y;
	glfwGetWindowPos(GetWindowByID(id).glfw, &x, &y);
	return (ThatX ?x:y);
}

/*Установить позицию окна*/
void SetWindowPosition(string id, int pos, bool ThatX) {
	int x; int y;
	Window window = GetWindowByID(id);
	glfwGetWindowPos(window.glfw, &x, &y);
	glfwSetWindowPos(window.glfw, (ThatX?pos:x), (!ThatX ? pos:y));
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
		PE("Window not found! GetWindowByID('" + id + "')", "E0020");
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
		PE("Window not found! GetIDByWindow()", "E0020");
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

/*Показывает или прячет курсор в окне*/
void ShowCursor_(string id, bool b) {
	glfwSetInputMode(GetWindowByID(id).glfw, GLFW_CURSOR, (b ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN));
}

/*Изменение авторазмера OpenGL*/
void SetWindowAutosize(string id, bool b) {
	Window w = GetWindowByID(id);
	w.AutoResize = b;
	Windows[id] = w;
}

/*Добавить сцену*/
void SetWindowScene(string id, string b, bool DontPrint) {
	if (!DontPrint) { P("SCENE", "Scene [" + b + "] applied to window [" + id + "]"); }
	Window w = GetWindowByID(id);
	Scene scene = GetScene(b);
	scene.windowid = id;
	Scenes[b] = scene;
	w.scene = b;
	Windows[id] = w;
	UpdateTexturesWindowCreated();
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

/*Установить прозрачность окна*/
void SetWindowTransparency(string id, int alpha) {
	Window w = GetWindowByID(id);
	w.Transparency = alpha;
	glfwSetWindowOpacity(w.glfw, float(alpha) / 255);
	Windows[id] = w;
}

/*Получить прозрачность окна*/
int GetWindowTransparency(string id) {
	Window w = GetWindowByID(id);
	return w.Transparency;
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

/*Нажатие клавиши в мышке в окне*/
void SetWindowMPEvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowMousePress = f;
	Windows[id] = w;
}

/*Отжатие клавиши в мышке в окне*/
void SetWindowMREvent(string id, sol::function f) {
	Window w = GetWindowByID(id);
	w.WindowMouseRelease = f;
	Windows[id] = w;
}

/*Зажатие клавиши в мышке в окне*/
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
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
			GLFWwindow* window = glfwCreateWindow(500, 500, StringToConstChar(title), NULL, NULL);
			if (!window) {
				PF("Window could not be created! CreateWindow('" + id + "'," + to_string(sizex) + "," + to_string(sizey) + ",'" + title + "')", "C0018");
				return Window();
			}
			glfwMakeContextCurrent(window);
			Window window_ = Window(id,window);
			CreateBuffers(window_);
			window_.StartSizeX = 500;
			window_.StartSizeY = 500;
			glfwSetKeyCallback(window, KeyCallback);
			glfwSetMouseButtonCallback(window, MouseCallback);

			Windows[id] = window_;
			Windows_2[window] = id;

			SetWindowSize(id, false, sizex);
			SetWindowSize(id, true, sizey);

			P("WINDOW", "Window [" + id + "] created!");

			UpdateTexturesWindowCreated();

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
		glDeleteVertexArrays(1, &window.Arrays);
		glDeleteBuffers(1, &window.Buffer);
		for (auto it = window.Shaders.begin(); it != window.Shaders.end(); ++it) {
			glDeleteProgram(it->second);
		}
		glfwDestroyWindow(window.glfw);
		P("WINDOW", "Window [" + id + "] destroyed!");
		Windows_2.erase(window.glfw);
		Windows.erase(id);
	}
}