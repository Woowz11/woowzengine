#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include <filesystem>
#include <chrono>
#include <thread>
#include <limits>
#include "LuaCompile.h"
#include "OpenGame.h"
#include "Base.h"
#include "Easyer.h"
#include "Files.h"
#include "GLFW.h"
#include "Cycles.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "RenderElement.h"

#include "l_Color.h"
#include "l_Vector2.h"
#include "l_Vertex.h"
//#include "l_Element.h"
#include "l_Sprite.h"
#include "l_Scene.h"

using namespace std;

sol::state lua{};

/*���� ����*/

/*��������� �������� �� lua ��� ���*/
void l_CheckLua() {
	P("LUACHK", "Lua supported!");
}

/*��������� ����������*/
void l_Exit() {
	Exit();
}

/*��������� ����������*/
void l_Wait(int milisec, sol::function func) {
	if (func.valid()) {
		auto wainfunc = [func, milisec]() {
				std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
				try {
					func();
				}
				catch (const sol::error& e) { /*��������� ������ �� lua ��������*/
					string what = e.what();
					PE(what, "LUA");
				}
			};
		std::thread thread(wainfunc);
		thread.detach();
	}
	else {
		Wait(milisec);
	}
}

/*������� ��������� ����� �� 0 �� 1*/
float l_Random(float min,float max) {
	if (min == 0 && max == 0) { return Random(); }
	return Random(min,max);
}

/*������� ����� � �����*/
int l_Round(float val) {
	return FloatToInt(val);
}

/*���� ����� ��� ���� � ���� �������*/
bool l_HasDirectory(string path) {
	return HasDirectory(path);
}

/*�������� ������ �� �����*/
string l_ReadFile(string path) {
	return ReadFile(path);
}

/*�������� ������ � ����*/
void l_WriteFile(string path, string value, bool Add) {
	if (SafeMode()) { PW("Function [WriteFile('"+path+"','"+value+"')] cannot be started in SafeMode!","LW0001"); }
	else {
		if (Add) {
			WriteToFile(path, ReadFile(path)+value);
		}
		else {
			WriteToFile(path, value);
		}
	}
}

/*���������� ��������� ��������� � �������*/
void l_PrintCustom(string module,string text, string firstsymbol, int color) {
		if (StringEmpty(module)) { PE("Provided module in PrintCustom('"+text+"') cannot be empty!", "L0003"); }
		else {
			if (color < 0 || color>255) { PE("The color must match 0<color<255 in PrintCustom('" + Uppercase(module) + "','" + text + "')!", "L0004"); }
			int color_ = color;
			if (color == 0) { color_ = 8; }
			if (color == 8) { color_ = 0; }
			else {
				char chr = ' ';
				if (!StringEmpty(firstsymbol)) {
					if (firstsymbol.size() > 1) {
						PW("Length of the first character '" + firstsymbol + "' is >1 in PrintCustom('" + Uppercase(module) + "','" + text + "')!", "LW0000");
					}
					chr = firstsymbol.at(0);
				}
				Print(ConvertTextToConsoleLogMessage(text, module, chr), color_);
			}
		}
}

/*���������� ��������� � �������*/
void l_Print(string text) {
	PP(text);
}

/*���������� �������������� � �������*/
void l_Warn(string text,int code) {
	
	PW(text,"GW"+FillString(to_string(code),4,'0',true));
}

/*���������� ������ � �������*/
void l_Error(string text,int code) {
	PE(text, "GE" + FillString(to_string(code), 4, '0', true));
}

/*���������� ��������� ������ � �������*/
void l_Fatal(string text, int code) {
	PF(text, "GF" + FillString(to_string(code), 4, '0', true));
}

/*���������� ��������� � ������� (��� ������� ����)*/
void l_PrintClear(string text, int color) {
	if (color < 0 || color>255) { PE("The color must match 0<color<255 in PrintClear('"+text+"')!","L0002"); }
	else {
		int color_ = color;
		if (color == 0) { color_ = 8; }
		if (color == 8) { color_ = 0; }
		Print(text, color_);
	}
}

/*���������� ������� � ����*/
void l_Cycle(sol::function func, int milisec) {
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for Cycle("+to_string(milisec)+")!","L0000"); }
	else {
		if (milisec <= 0) { PE("The time in Cycle(" + to_string(milisec) + ") cannot be <= 0!","L0001"); }
		else {
			SetCycleFunction(func, milisec);
		}
	}
}

/*������� �����*/
void l_CreateDirectory(string path) {
	if (SafeMode()) { PW("Function [CreateDirectory('" + path + "')] cannot be started in SafeMode!", "LW0002"); }
	else {
		GetOrCreateFolder(path);
	}
}

/*������� ����*/
void l_CreateFile(string pathandname, string source) {
	if (SafeMode()) { PW("Function [CreateFile('" + pathandname + "','" + source + "')] cannot be started in SafeMode!", "LW0003"); }
	else {
		GetOrCreateFile(pathandname);
		if (!StringEmpty(source)) {
			WriteToFile(pathandname, source);
		}
	}
}

/*������������ ����*/
void l_RenameFile(string path, string newname) {
	if (SafeMode()) { PW("Function [RenameFile('" + path + "','" + newname + "')] cannot be started in SafeMode!", "LW0008"); }
	else {
		if (!HasDirectory(path)) { PE("File not found! RenameFile('"+path+"','"+newname+"')","L0007"); }
		else {
			RenameFile(path,GetPathWithoutFileName(path)+newname);
		}
	}
}

/*������ ���������� � JSON*/
void l_WriteJSON(string path, string id, string val) {
	if (SafeMode()) { PW("Function [WriteJSON('" + path + "','" + id + "','"+val+"')] cannot be started in SafeMode!", "LW0004"); }
	else {
		WriteToJson(path, id, val);
	}
}

/*�������� ������ �� json �����*/
string l_ReadJSON(string path, string id) {
	if (HasDirectory(path)) { PE("Json file not found! ReadJSON('"+path+"','"+id+"')", "L0006"); return "ERROR"; }
	if(!JSONValid(path)){ PE("Json file corrupted! ReadJSON('" + path + "','" + id + "')","L0005"); return "ERROR"; }
	return ReadJson(path,id);
}

/*������ ����� �������������*/
float l_Abs(float f) {
	return abs(f);
}

/*���������� ������������ �����*/
float l_Max(float f, float f2) {
	if (f == f2) { PW("In the Max("+to_string(f) + "," + to_string(f2) + ") function, the numbers cannot be the same!", "LW0006"); return f; }
	return fmax(f,f2);
}

/*���������� ����������� �����*/
float l_Min(float f, float f2) {
	if (f == f2) { PW("In the Min(" + to_string(f) + "," + to_string(f2) + ") function, the numbers cannot be the same!", "LW0007"); return f; }
	return fmin(f, f2);
}

/*���������� ������*/
float l_Sqrt(float f) {
	return sqrt(f);
}

/*��������*/
float l_Log(float f, float base) {
	return log(f)/log(base);
}

/*�������� �����������*/
float l_LogE(float f) {
	return log(f);
}

/*�������� �� ������ 10*/
float l_Log10(float f) {
	return log10(f);
}

/*�����*/
float l_Sin(float f) {
	return sin(f);
}

/*�������*/
float l_Cos(float f) {
	return cos(f);
}

/*��������� � ������� �������*/
int l_Floor(float f) {
	return floor(f);
}

/*��������� � ������� �������*/
int l_Ceil(float f) {
	return ceil(f);
}

/*������*/
float l_Tan(float f) {
	return tan(f);
}

/*�����*/
float l_Sum(float f, float f2) {
	return f + f2;
}

/*���������*/
float l_Sub(float f, float f2) {
	return f - f2;
}

/*���������*/
float l_Mul(float f, float f2) {
	return f * f2;
}

/*�������*/
float l_Div(float f, float f2) {
	if (f2 == 0) { PW("Can't divide by 0! Div("+to_string(f) + "," + to_string(f2) + ")", "LW0005"); return 0; }
	return f / f2;
}

/*����������� < ����� < ������������*/
float l_Clamp(float min_, float f, float max_) {
	return fmax(fmin(f, max_), min_);
}

/*��������*/
float l_ASin(float f) {
	return asin(f);
}

/*����������*/
float l_ACos(float f) {
	return acos(f);
}

/*���������*/
float l_ATan(float f) {
	return atan(f);
}

/*���������� ������*/
float l_Cbrt(float f) {
	return cbrt(f);
}

/*�������������� ��������*/
float l_Exp(float f) {
	return exp(f);
}

/*�������������� �������� � ����� 1*/
float l_ExpM(float f) {
	return expm1(f);
}

/*���������� �������� ��������� ��������*/
float l_Fabs(float f) {
	return fabs(f);
}

/*�������� ����� ������� (�������������)*/
float l_FDim(float f,float f2) {
	return fdim(f,f2);
}

/*���������� ������ �� (x^2 +y^2)*/
float l_Hypot(float f, float f2) {
	return hypot(f,f2);
}

/*x*y+z*/
float l_Fma(float x,float y,float z) {
	return fma(x,y,z);
}

/*������� �� �������*/
float l_Mod(float f, float f2) {
	return fmod(f, f2);
}

/*������� �� ������� �� ��������*/
float l_Rem(float f, float f2) {
	return remainder(f, f2);
}

/*�������� � �������*/
float l_Pow(float f, float f2) {
	return pow(f,f2);
}

/*��������������� �����*/
float l_HSin(float f) {
	return sinh(f);
}

/*��������������� �������*/
float l_HCos(float f) {
	return cosh(f);
}

/*��������������� ������*/
float l_HTan(float f) {
	return tanh(f);
}

/*�������� ������� �����*/
float l_Trunc(float f) {
	return trunc(f);
}

/*��������� �������� � ��������� ������� �� ������� � ����� �����*/
float l_Modf(float f, float * f2) {
	return modff(f, f2);
}

/*���� ������ � ������*/
bool l_HasString(string Str, string whatneedfound) {
	if (Str == "") { PE("String cannot be empty! HasString('','"+whatneedfound+"')", "L0012"); return false; }
	if (whatneedfound == "") { PW("The string to be found cannot be empty! HasString('" + Str + "','')", "LW0010"); return true; }
	return StringHasString(Str, whatneedfound);
}

/*������ ����� � ������*/
string l_Replace(string Str, string that, string tothat) {
	if (Str == "") { PE("String cannot be empty! Replace('','"+that+"','"+tothat+"')", "L0010"); return "ERROR_L0010"; }
	if (that == "") { PE("Replace string cannot be empty! Replace('" + Str + "','','" + tothat + "')","L0008"); return "ERROR_L0008"; }
	if (that == tothat) { PW("Replace strings can't be the same! Replace('"+Str+"','"+that+"','"+tothat+"')","LW0009"); return Str; }
	return ReplaceString(Str, that, tothat);
}

/*������� ������ �� ������*/
string l_Remove(string Str, string that) {
	if (Str == "") { PE("String cannot be empty! Remove('','" + that + "')", "L0011"); return "ERROR_L0011"; }
	if (that == "") { PE("Remove string cannot be empty! Remove('" + Str + "','" + that + "')", "L0009"); return "ERROR_L0009"; }
	return ReplaceString(Str, that, "");
}

/*�������� ������� ��������*/
sol::table l_Charcters(string Str) {
	if (Str == "") { PW("String cannot be empty! Charcters('')","LW0011"); return lua.create_table(); }
	sol::table tbl = lua.create_table();
	for (char& c : Str) {
		tbl.add(CharToString(c));
	}
	return tbl;
}

/*������ ������ ���������*/
string l_Uppercase(string Str) {
	if (Str == "") { PW("String cannot be empty! Uppercase('')", "LW0012"); return ""; }
	return Uppercase(Str);
}

/*������ ������ �� ���������*/
string l_Lowercase(string Str) {
	if (Str == "") { PW("String cannot be empty! Lowercase('')", "LW0013"); return ""; }
	return Lowercase(Str);
}

/*�������� ������ ������*/
int l_Length(string Str) {
	return Str.length();
}

/*�������� ����� ������*/
string l_SubStr(string Str, int pos, int size) {
	return Str.substr(pos, size);
}

/*���������� ������ � �����*/
float l_ToNumber(string Str) {
	return StringToFloat(Str);
}

/*������ ������� cmd*/
void l_Cmd(string command) {
	if (SafeMode()) { PW("Function [Cmd('" + command + "')] cannot be started in SafeMode!", "LW0014"); }
	else {
		system(StringToConstChar(command));
	}
}

/*������� ����*/
void l_CreateWindow(string id, string Title, int sizex, int sizey) {
	int x = sizex;
	int y = sizey;
	if (sizex == 0) { x = 640; }
	if (sizey == 0) { y = 480; }
	CreateWindowGLFW(id, x, y, Title);
}

/*���������� ����*/
void l_DestroyWindow(string id) {
	DestroyWindowGLFW(id);
}

/*��������� ���� �� ���� � ������*/
bool l_HasWindow(string id) {
	return HasWindow(id);
}

/*������ ���� �������*/
void l_SetWindowMain(string id) {
	SetWindowToMain(id);
}

/*�������� ���� �������� ����*/
string l_MainWindow() {
	return GetSessionInfo("MainWindow");
}

/*�������� ���*/
int l_Seed() {
	return StringToInt(GetSessionInfo("Seed"));
}

/*���������� ���*/
void l_SetSeed(int seed) {
	SetRandomSeed(seed);
}

/*�������� ������ ���� X*/
int l_GetWindowX(string id) {
	return GetWindowSize(id, false);
}

/*�������� ������ ���� Y*/
int l_GetWindowY(string id) {
	return GetWindowSize(id, true);
}

/*�������� ������ ������ �� X*/
void l_SetWindowX(string id, int i) {
	SetWindowSize(id, false, i);
}

/*�������� ������ ������ �� Y*/
void l_SetWindowY(string id, int i) {
	SetWindowSize(id, true, i);
}

/*�������� �������� ����*/
void l_SetWindowTitle(string id, string title) {
	SetWindowTitle(id, title);
}

/*��������� ����������� � ����*/
void l_SetWindowAutoSize(string id, bool b) {
	SetWindowAutosize(id, b);
}

/*��������� �������� � ����*/
void l_SetWindowScale(string id, float f) {
	SetWindowScale(id, f);
}

/*����� ABS*/
float l_AbsSin(float f) {
	return abs(sin(f));
}

/*������� ABS*/
float l_AbsCos(float f) {
	return abs(cos(f));
}

/*(�����+1)/2*/
float l_DSin(float f) {
	return (sin(f) + 1) / 2;
}

/*(�������+1)/2*/
float l_DCos(float f) {
	return (cos(f) + 1) / 2;
}

/*��������� ��� ��������� ������ ������ ����*/
void l_SetWindowResizable(string id, bool b) {
	SetWindowResizable(id, b);
}

/*�������� ������� ����� ���� �����������*/
void l_SetWindowEventClosed(string id, sol::function func) {
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for SetWindowEventClosed(" + id + ")!", "L0013"); }
	else {
		SetWindowClosedEvent(id, func);
	}
}

/*�������� ������� ����� ���� �����������*/
void l_SetEventClosed(sol::function func) {
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for SetEventClosed()!", "L0014"); }
	else {
		SetGameClosedEvent(func);
	}
}

/*������� ������� � ����*/
void l_SetWindowEventKeyPress(string id, sol::function func) {
	SetWindowKPEvent(id, func);
}

/*������� ������� � ����*/
void l_SetWindowEventKeyRelease(string id, sol::function func) {
	SetWindowKREvent(id, func);
}

/*������� ������� � ����*/
void l_SetWindowEventKeyHold(string id, sol::function func) {
	SetWindowKHEvent(id, func);
}

/*������� ������� ������*/
sol::table l_PressedKeys() {
	sol::table tbl = lua.create_table();
	for (const auto& p : GetPressedKeys()) {
		tbl.add(p.first);
	}
	return tbl;
}

/*��������� ����� � ����*/
void l_SetWindowScene(string id, l_Scene& scene) {
	scene.windowid = id;
	SetWindowScene(id, scene.ToCPP());
}

/*�������� ���� ���� � ������� �����*/
string l_GetSceneWindow(l_Scene scene) {
	return scene.windowid;
}

/*������� ��������� ����� �� 0 �� 1 (�����)*/
int l_RRandom(int min, int max) {
	if (min == 0 && max == 0) { return round(Random()); }
	return round(Random(min, max));
}

/*���� woowzengine*/

void StartFunction(sol::function func, string s) {
	if (func != sol::nil && func.valid()) {
		try {
			func(s);
		}
		catch (const sol::error& e) { /*��������� ������ �� lua ��������*/
			string what = e.what();
			PE(what, "LUA");
		}
	}
}

void CompileScript(string Path) {
	try {
		lua.script_file(Path);
	}
	catch (const sol::error& e) { /*��������� ������ �� lua ��������*/
		string what = e.what();
		PE(what, "LUA");
	}
}

void LuaCompile() {
	P("LUA", "Start Lua compile...");
	lua.open_libraries(sol::lib::package);

	/*�������� �������� �� ����*/
	const string package_path_ = lua["package"]["path"];
	lua["package"]["path"] = package_path_ + (!package_path_.empty() ? ";" : "") + GetSessionInfo("SourcePath") + "?.lua";
	for (auto& p : std::filesystem::recursive_directory_iterator(GetSessionInfo("SourcePath"))) {
		if (p.is_directory()) {
			const string package_path = lua["package"]["path"];
			lua["package"]["path"] = package_path + (!package_path.empty() ? ";" : "") + p.path().string() + "/?.lua";
		}
	}
	CreateValueJson(GetSessionInfo("SessionPath"), "LuaLibs", lua["package"]["path"]);
	P("LUA", "Lua libraries are loaded!");

	/*������������*/
	lua.new_usertype<l_Color>("Color",
		sol::constructors<l_Color(), l_Color(int, int, int, int), l_Color(int, int, int)>(),
		"r", &l_Color::r,
		"g", &l_Color::g,
		"b", &l_Color::b,
		"a", &l_Color::a
	);

	lua.new_usertype<l_Vector2>("Vector2",
		sol::constructors<l_Vector2(), l_Vector2(float, float)>(),
		"x", &l_Vector2::x,
		"y", &l_Vector2::y
	);

	lua.new_usertype<l_Vertex>("Vertex",
		sol::constructors<l_Vertex(), l_Vertex(l_Vector2, l_Color), l_Vertex(l_Vector2)>(),
		"Color", &l_Vertex::Color,
		"Position", &l_Vertex::Position
	);

	/*lua.new_usertype<l_Element>("Element",
		sol::constructors<l_Element(), l_Element(string)>(),
		"SetType", &l_Element::SetTypeLua
	);*/

	lua.new_usertype<l_Sprite>("Sprite",
		sol::constructors<l_Sprite(), l_Sprite(string,string,l_Vector2)>(),
		"Position", &l_Sprite::position,
		"Orientation", &l_Sprite::angle,
		"Color", &l_Sprite::color,
		"ZIndex", &l_Sprite::zindex,
		"Scale", &l_Sprite::size,
		"Texture", &l_Sprite::texture,
		"Origin", &l_Sprite::origin,
		"ThatUI", &l_Sprite::movewithcamera
	);

	lua.new_usertype<l_Scene>("Scene",
		sol::constructors<l_Scene(), l_Scene(string)>(),
		//"AddElement", &l_Scene::AddElement,
		"SetBackgroundColor", &l_Scene::SetBackgroundColor,
		"GetBackgroundColor", &l_Scene::GetBackgroundColor
	);

	/*���������*/
	lua["Pi"] = sol::as_table(3.14159265358979323846);
	lua["Sqrt2"] = sol::as_table(1.41421356237309504880);
	lua["E"] = sol::as_table(2.71828182845904523536);
	lua["Log2E"] = sol::as_table(1.44269504088896340736);
	lua["Log10E"] = sol::as_table(0.434294481903251827651);
	lua["Ln2"] = sol::as_table(0.693147180559945309417);
	lua["Ln10"] = sol::as_table(2.30258509299404568402);
	string sourcepath = GetSessionInfo("GamePath");
	sourcepath = sourcepath.substr(0, sourcepath.size() - 1);
	lua["SourcePath"] = sol::as_table(sourcepath);
	lua["GamePath"] = sol::as_table(sourcepath + "/game");
	lua["EnginePath"] = sol::as_table(sourcepath + "/woowzengine");
	lua["EngineVersion"] = sol::as_table(GetSessionInfo("Version"));
	lua["LogPath"] = sol::as_table(GetSessionInfo("Log"));
	lua["Version"] = sol::as_table(GetGameInfo("Version"));
	lua["GameName"] = sol::as_table(GetGameInfo("Name"));
	lua["Author"] = sol::as_table(GetGameInfo("Author"));
	lua["SafeMode"] = sol::as_table(StringToBool(GetEngineInfo("SafeMode")));
	lua["ConsoleEnabled"] = sol::as_table(StringToBool(GetEngineInfo("Console")));
	lua["DebugMode"] = sol::as_table(StringToBool(GetSessionInfo("Debug")));
	lua["Infinity"] = sol::as_table(std::numeric_limits<int>::max());
	lua["Red"] = sol::as_table(l_Color(255, 0, 0, 255));
	lua["Green"] = sol::as_table(l_Color(0, 255, 0, 255));
	lua["Blue"] = sol::as_table(l_Color(0, 0, 255, 255));
	lua["Yellow"] = sol::as_table(l_Color(255, 255, 0, 255));
	lua["White"] = sol::as_table(l_Color(255, 255, 255, 255));
	lua["Black"] = sol::as_table(l_Color(0, 0, 0, 255));
	lua["Gray"] = sol::as_table(l_Color(125, 125, 125, 255));
	lua["Orange"] = sol::as_table(l_Color(255, 125, 0, 255));
	lua["Purple"] = sol::as_table(l_Color(0, 125, 255, 255));
	lua["Cyan"] = sol::as_table(l_Color(125, 125, 255, 255));
	lua["Transparent"] = sol::as_table(l_Color(0, 0, 0, 0));
	lua["Up"] = sol::as_table(l_Vector2(0, 1));
	lua["Down"] = sol::as_table(l_Vector2(0, -1));
	lua["Right"] = sol::as_table(l_Vector2(1, 0));
	lua["Left"] = sol::as_table(l_Vector2(-1, 0));

	/*�������*/
	lua.set_function("CheckLua", &l_CheckLua);
	lua.set_function("Wait", &l_Wait);
	lua.set_function("Exit", &l_Exit);
	lua.set_function("PrintCustom", &l_PrintCustom);
	lua.set_function("Print", &l_Print);
	lua.set_function("Warn", &l_Warn);
	lua.set_function("Error", &l_Error);
	lua.set_function("Fatal", &l_Fatal);
	lua.set_function("PrintClear", &l_PrintClear);
	lua.set_function("Cycle", &l_Cycle);
	lua.set_function("Random", &l_Random);
	lua.set_function("Round", &l_Round);
	lua.set_function("HasDirectory", &l_HasDirectory);
	lua.set_function("ReadFile", &l_ReadFile);
	lua.set_function("WriteFile", &l_WriteFile);
	lua.set_function("CreateDirectory", &l_CreateDirectory);
	lua.set_function("CreateFile", &l_CreateFile);
	lua.set_function("RenameFile", &l_RenameFile);
	lua.set_function("ReadJSON", &l_ReadJSON);
	lua.set_function("WriteJSON", &l_WriteJSON);
	lua.set_function("Abs", &l_Abs);
	lua.set_function("Min", &l_Min);
	lua.set_function("Max", &l_Max);
	lua.set_function("Sqrt", &l_Sqrt);
	lua.set_function("Log", &l_Log);
	lua.set_function("LogE", &l_LogE);
	lua.set_function("Log10", &l_Log10);
	lua.set_function("Sin", &l_Sin);
	lua.set_function("Cos", &l_Cos);
	lua.set_function("Floor", &l_Floor);
	lua.set_function("Ceil", &l_Ceil);
	lua.set_function("Tan", &l_Tan);
	lua.set_function("Sum", &l_Sum);
	lua.set_function("Sub", &l_Sub);
	lua.set_function("Mul", &l_Mul);
	lua.set_function("Div", &l_Div);
	lua.set_function("Clamp", &l_Clamp);
	lua.set_function("ASin", &l_ASin);
	lua.set_function("ACos", &l_ACos);
	lua.set_function("ATan", &l_ATan);
	lua.set_function("Cbrt", &l_Cbrt);
	lua.set_function("Exp", &l_Exp);
	lua.set_function("ExpM", &l_ExpM);
	lua.set_function("Fabs", &l_Fabs);
	lua.set_function("FDim", &l_FDim);
	lua.set_function("Hypot", &l_Hypot);
	lua.set_function("Fma", &l_Fma);
	lua.set_function("Mod", &l_Mod);
	lua.set_function("Rem", &l_Rem);
	lua.set_function("Pow", &l_Pow);
	lua.set_function("HSin", &l_HSin);
	lua.set_function("HCos", &l_HCos);
	lua.set_function("HTan", &l_HTan);
	lua.set_function("Trunc", &l_Trunc);
	lua.set_function("Modf", &l_Modf);
	lua.set_function("HasString", &l_HasString);
	lua.set_function("Replace", &l_Replace);
	lua.set_function("Remove", &l_Remove);
	lua.set_function("Charcters", &l_Charcters);
	lua.set_function("Uppercase", &l_Uppercase);
	lua.set_function("Lowercase", &l_Lowercase);
	lua.set_function("Length", &l_Length);
	lua.set_function("SubStr", &l_SubStr);
	lua.set_function("ToNumber", &l_ToNumber);
	lua.set_function("Cmd", &l_Cmd);
	lua.set_function("CreateWindow", &l_CreateWindow);
	lua.set_function("DestroyWindow", &l_DestroyWindow);
	lua.set_function("HasWindow", &l_HasWindow);
	lua.set_function("SetWindowMain", &l_SetWindowMain);
	lua.set_function("MainWindow", &l_MainWindow);
	lua.set_function("Seed", &l_Seed);
	lua.set_function("SetSeed", &l_SetSeed);
	lua.set_function("GetWindowX", &l_GetWindowX);
	lua.set_function("GetWindowY", &l_GetWindowY);
	lua.set_function("SetWindowX", &l_SetWindowX);
	lua.set_function("SetWindowY", &l_SetWindowY);
	lua.set_function("SetWindowTitle", &l_SetWindowTitle);
	lua.set_function("SetWindowAutoSize", &l_SetWindowAutoSize);
	lua.set_function("SetWindowScale", &l_SetWindowScale);
	lua.set_function("AbsSin", &l_AbsSin);
	lua.set_function("AbsCos", &l_AbsCos);
	lua.set_function("DSin", &l_DSin);
	lua.set_function("DCos", &l_DCos);
	lua.set_function("SetWindowResizable", &l_SetWindowResizable);
	lua.set_function("SetWindowEventClosed", &l_SetWindowEventClosed);
	lua.set_function("SetEventClosed", &l_SetEventClosed);
	lua.set_function("SetWindowEventKeyPress", &l_SetWindowEventKeyPress);
	lua.set_function("SetWindowEventKeyRelease", &l_SetWindowEventKeyRelease);
	lua.set_function("SetWindowEventKeyHold", &l_SetWindowEventKeyHold);
	lua.set_function("PressedKeys", &l_PressedKeys);
	lua.set_function("SetWindowScene", &l_SetWindowScene);
	lua.set_function("GetSceneWindow", &l_GetSceneWindow);
	lua.set_function("RRandom", &l_RRandom);

	P("LUA", "Lua functions and etc. are loaded!");
	P("LUA", "Start start.lua script...");
	CompileScript(GetSessionInfo("SourcePath") + "start.lua");
}