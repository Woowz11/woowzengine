﻿#pragma warning(disable : 4244)
#pragma warning(disable : 4267)

#define _CRT_SECURE_NO_WARNINGS

#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include <filesystem>
#include <chrono>
#include <thread>
#include <limits>
#include <any>
#include "LuaCompile.h"
#include "OpenGame.h"
#include "Base.h"
#include "Easyer.h"
#include "Files.h"
#include "GLFW.h"
#include "Cycles.h"
#include "Console.h"
#include "Discord.h"
#include "WindowsElements.h"
#include "WConst.h"
#include "fcntl.h"
#include "io.h"
#include "stdio.h"
#include "Enum.h"
#include "ImGui_.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "RenderElement.h"
#include "Texture.h"

#include "l_Color.h"
#include "l_Vector2.h"
#include "l_Vector3.h"
#include "l_Vector4.h"
#include "l_Sound.h"
#include "l_Vertex.h"
#include "l_Sprite.h"

using namespace std;

sol::state lua{};

/*Зона игры*/

string EmptyWindow = "New Window";
string EmptyScene = "New Scene";
string EmptySprite = "New Sprite";
string EmptyTexture = "New Texture";
string EmptyText = "New Text";
string EmptyShader = "default";
string EmptyFont = "default";
string EmptyImGuiWindow = "New ImGui Window";
string EmptyImGuiElement = "New ImGui Element";
string EmptyDUser = "495215150165524481";
string EmptyImage;

/*Проверяет работает ли lua или нет*/
void l_CheckLua() {
	DiscordTest();
	P("LUACHK", "Lua supported!");
}

/*Закрывает приложение*/
void l_Exit() {
	Exit();
}

/*Закрывает приложение*/
void l_Wait(int milisec, sol::function func) {
	if (func.valid()) {
		auto wainfunc = [func, milisec]() {
				std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
				try {
					func();
				}
				catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
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

/*Вернуть случайное число от 0 до 1*/
float l_Random(float min,float max) {
	if (min == 0 && max == 0) { return Random(); }
	return Random(min,max);
}

/*Дробное число в целое*/
int l_Round(float val) {
	return FloatToInt(val);
}

/*Есть папка или файл в этой позиции*/
bool l_HasDirectory(sol::object path) {
	return HasDirectory(ToString(path));
}

/*Получить данные из файла*/
string l_ReadFile(sol::object path) {
	return ReadFile(ToString(path));
}

/*Записать данные в файл*/
void l_WriteFile(sol::object path_, sol::object value_, bool Add) {
	string path = ToString(path_);
	string value = ToString(value_);
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

/*Отправляет кастомное сообщение в консоль*/
void l_PrintCustom(sol::object module, sol::object text, sol::object firstsymbol, int color) {
		if (module == sol::nil) { PE("Provided module in PrintCustom('"+ToString(text)+"') cannot be empty!", "L0003"); }
		else {
			if (color < 0 || color>255) { PE("The color must match color <= 255 & color >= 0 in PrintCustom('" + Uppercase(ToString(module)) + "','" + ToString(text) + "')!", "L0004"); }
			int color_ = color;
			if (color == 0) { color_ = 8; }
			if (color == 8) { color_ = 0; }
			else {
				string chr = " ";
				if (firstsymbol!=sol::nil) {
					if (!StringEmpty(ToString(firstsymbol))) {
						if (ToString(firstsymbol).size() > 1) {
							PW("Length of the first character '" + ToString(firstsymbol) + "' is > 1 in PrintCustom('" + Uppercase(ToString(module)) + "','" + ToString(text) + "')!", "LW0000");
						}
						chr = ToString(firstsymbol).at(0);
					}
				}
				P(ToString(module), ToString(text), color_, chr);
			}
		}
}

/*Отправляет сообщение в консоль*/
void l_Print(sol::object text) {
	PP(ToString(text));
}

/*Отправляет предупреждение в консоль*/
void l_Warn(sol::object text, sol::object code) {
	
	PW(ToString(text),"GW"+FillString(ToString(code,"_EMPTY"), 4, '0', true));
}

/*Отправляет ошибку в консоль*/
void l_Error(sol::object text, sol::object code) {
	PE(ToString(text), "GE" + FillString(ToString(code, "_EMPTY"), 4, '0', true));
}

/*Отправляет фатальную ошибку в консоль*/
void l_Fatal(sol::object text, sol::object code) {
	PF(ToString(text), "GF" + FillString(ToString(code, "_EMPTY"), 4, '0', true));
}

/*Отправляет сообщение в консоль (без формата лога)*/
void l_PrintClear(sol::object text, int color) {
	if (color < 0 || color>255) { PE("The color must match 0<color<255 in PrintClear('"+ToString(text)+"')!","L0002"); }
	else {
		int color_ = color;
		if (color == 0) { color_ = 8; }
		if (color == 8) { color_ = 0; }
		Print(ToString(text), color_);
	}
}

/*Превращает функцию в цикл*/
void l_Cycle(sol::function func, int milisec) {
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for Cycle("+to_string(milisec)+")!","L0000"); }
	else {
		if (milisec <= 0) { PE("The time in Cycle(" + to_string(milisec) + ") cannot be <= 0!","L0001"); }
		else {
			SetCycleFunction(func, milisec);
		}
	}
}

/*Создать папки*/
void l_CreateDirectory(sol::object path_) {
	string path = ToString(path_);
	if (SafeMode()) { PW("Function [CreateDirectory('" + path + "')] cannot be started in SafeMode!", "LW0002"); }
	else {
		GetOrCreateFolder(path);
	}
}

/*Создать файл*/
void l_CreateFile(sol::object pathandname_, sol::object source_) {
	string pathandname = ToString(pathandname_);
	string source = ToString(source_);
	if (SafeMode()) { PW("Function [CreateFile('" + pathandname + "','" + source + "')] cannot be started in SafeMode!", "LW0003"); }
	else {
		bool has = HasDirectory(pathandname);
		GetOrCreateFile(pathandname);
		if (!StringEmpty(source)&&!has) {
			WriteToFile(pathandname, source);
		}
	}
}

/*Переменовать файл*/
void l_RenameFile(sol::object path_, sol::object newname_) {
	string path = ToString(path_);
	string newname = ToString(newname_);
	if (SafeMode()) { PW("Function [RenameFile('" + path + "','" + newname + "')] cannot be started in SafeMode!", "LW0008"); }
	else {
		if (!HasDirectory(path)) { PE("File not found! RenameFile('"+path+"','"+newname+"')","L0007"); }
		else {
			RenameFile(path,GetPathWithoutFileName(path)+newname);
		}
	}
}

/*Запись переменных в JSON*/
void l_WriteJSON(sol::object path_, sol::object id_, sol::object val_) {
	string path = ToString(path_);
	string id = ToString(id_,"Key");
	string val = ToString(val_,"Value");
	if (SafeMode()) { PW("Function [WriteJSON('" + path + "','" + id + "','"+val+"')] cannot be started in SafeMode!", "LW0004"); }
	else {
		WriteToJson(path, id, val);
	}
}

/*Получить данные из json файла*/
string l_ReadJSON(sol::object path_, sol::object id_) {
	string path = ToString(path_);
	string id = ToString(id_,"Key");
	if (!HasDirectory(path)) { PE("Json file not found! ReadJSON('"+path+"','"+id+"')", "L0006"); return "ERROR_L0006"; }
	if(!JSONValid(path)){ PE("Json file corrupted! ReadJSON('" + path + "','" + id + "')","L0005"); return "ERROR_L0005"; }
	return ReadJson(path,id);
}

/*Делает число положительным*/
float l_Abs(float f) {
	return abs(f);
}

/*Возвращает максимальное число*/
float l_Max(float f, float f2) {
	if (f == f2) { PW("In the Max("+to_string(f) + "," + to_string(f2) + ") function, the numbers cannot be the same!", "LW0006"); return f; }
	return fmax(f,f2);
}

/*Возвращает минимальное число*/
float l_Min(float f, float f2) {
	if (f == f2) { PW("In the Min(" + to_string(f) + "," + to_string(f2) + ") function, the numbers cannot be the same!", "LW0007"); return f; }
	return fmin(f, f2);
}

/*Квадратный корень*/
float l_Sqrt(float f) {
	if (f < 0) {
		PE("It is not possible to get the square root of a negative number! Sqrt("+to_string(f)+")", "L0040");
		return 0;
	}
	return sqrt(f);
}

/*Логарифм*/
float l_Log(float f, float base) {
	if (base == 0) { base = 2; }
	return log(f)/log(base);
}

/*Логарифм натуральный*/
float l_LogE(float f) {
	return log(f);
}

/*Логарифм на основе 10*/
float l_Log10(float f) {
	return log10(f);
}

/*Синус*/
float l_Sin(float f) {
	return sin(f);
}

/*Косинус*/
float l_Cos(float f) {
	return cos(f);
}

/*Округлить в меньшую сторону*/
int l_Floor(float f) {
	return floor(f);
}

/*Округлить в большую сторону*/
int l_Ceil(float f) {
	return ceil(f);
}

/*Тангенс*/
float l_Tan(float f) {
	if (cos(f)==0) {
		PE("Cosine should not be = 0! Tan("+to_string(f)+")", "L0042");
		return 0;
	}
	return tan(f);
}

/*Сумма*/
float l_Sum(float f, float f2) {
	return f + f2;
}

/*Вычитание*/
float l_Sub(float f, float f2) {
	return f - f2;
}

/*Умножение*/
float l_Mul(float f, float f2) {
	return f * f2;
}

/*Деление*/
float l_Div(float f, float f2) {
	if (f2 == 0) { PW("Can't divide by 0! Div("+to_string(f) + "," + to_string(f2) + ")", "LW0005"); return 0; }
	return f / f2;
}

/*Минимальное < число < максимальное*/
float l_Clamp(float f, float min_, float max_) {
	return fmax(fmin(f, max_), min_);
}

/*Арксинус*/
float l_ASin(float f) {
	return asin(f);
}

/*Арккосинус*/
float l_ACos(float f) {
	return acos(f);
}

/*Арктангенс*/
float l_ATan(float f) {
	return atan(f);
}

/*Кубический корень*/
float l_Cbrt(float f) {
	return cbrt(f);
}

/*Математическое ожидание*/
float l_Exp(float f) {
	return exp(f);
}

/*Математическое ожидание и минус 1*/
float l_ExpM(float f) {
	return expm1(f);
}

/*Абсолютное значение плавающей величины*/
float l_Fabs(float f) {
	return fabs(f);
}

/*Разность между числами (положительная)*/
float l_FDim(float f,float f2) {
	return fdim(f,f2);
}

/*Квадратный корень из (x^2 +y^2)*/
float l_Hypot(float f, float f2) {
	return hypot(f,f2);
}

/*x*y+z*/
float l_Fma(float x,float y,float z) {
	return fma(x,y,z);
}

/*Остаток от деления*/
float l_Mod(float f, float f2) {
	if (f2 == 0) {
		PE("Second number cannot be = 0! Mod("+to_string(f) + ",0)", "L0039");
		return 0;
	}
	return fmod(f, f2);
}

/*Гиперболический синус*/
float l_HSin(float f) {
	return sinh(f);
}

/*Гиперболический косинус*/
float l_HCos(float f) {
	return cosh(f);
}

/*Гиперболический тангец*/
float l_HTan(float f) {
	return tanh(f);
}

/*Удаление дробной части*/
int l_Trunc(float f) {
	return floorf(f - GetFractionalPart(f));
}

/*Ищет строку в строке*/
bool l_HasString(sol::object Str_, sol::object whatneedfound_) {
	string Str = ToString(Str_,"");
	string whatneedfound = ToString(whatneedfound_,"");
	if (Str == "") { PE("String cannot be empty! HasString('','"+whatneedfound+"')", "L0012"); return false; }
	if (whatneedfound == "") { PW("The string to be found cannot be empty! HasString('" + Str + "','')", "LW0010"); return true; }
	return StringHasString(Str, whatneedfound);
}

/*Замена строк в строке*/
string l_Replace(sol::object Str_, sol::object that_, sol::object tothat_) {
	string Str = ToString(Str_,"");
	string that = ToString(that_, "");
	string tothat = ToString(tothat_, "");
	if (Str == "") { PE("String cannot be empty! Replace('','"+that+"','"+tothat+"')", "L0010"); return "ERROR_L0010"; }
	if (that == "") { PE("Replace string cannot be empty! Replace('" + Str + "','','" + tothat + "')","L0008"); return "ERROR_L0008"; }
	if (that == tothat) { PW("Replace strings can't be the same! Replace('"+Str+"','"+that+"','"+tothat+"')","LW0009"); return Str; }
	return ReplaceString(Str, that, tothat);
}

/*Удалить строку из строки*/
string l_Remove(sol::object Str_, sol::object that_) {
	string Str = ToString(Str_, "");
	string that = ToString(that_, "");
	if (Str == "") { PE("String cannot be empty! Remove('','" + that + "')", "L0011"); return "ERROR_L0011"; }
	if (that == "") { PE("Remove string cannot be empty! Remove('" + Str + "','" + that + "')", "L0009"); return "ERROR_L0009"; }
	return ReplaceString(Str, that, "");
}

/*Получить таблицу символов*/
sol::table l_Charcters(sol::object Str_) {
	string Str = ToString(Str_, "");
	if (Str == "") { PW("String cannot be empty! Charcters('')","LW0011"); return lua.create_table(); }
	sol::table tbl = lua.create_table();
	int i = 0;
	for (wchar_t& c : StringToWString(Str)) {
		i++;
		tbl.set(i,WCharToWString(c));
	}
	return tbl;
}

/*Делает строку заглавной*/
string l_Uppercase(sol::object Str_) {
	string Str = ToString(Str_, "");
	if (Str == "") { PW("String cannot be empty! Uppercase('"+Str+"')", "LW0012"); return ""; }
	return Uppercase(Str);
}

/*Делает строку не заглавной*/
string l_Lowercase(sol::object Str_) {
	string Str = ToString(Str_, "");
	if (Str == "") { PW("String cannot be empty! Lowercase('"+Str+"')", "LW0013"); return ""; }
	return Lowercase(Str);
}

/*Получает размер строки или таблицы*/
int l_Length(sol::object obj) {
	if (obj.get_type() == sol::type::string) {
		return obj.as<std::wstring>().length();
	}else if(obj.get_type() == sol::type::table) {
		int size = 0;
		for (auto& kv : obj.as<sol::table>()) {
			size = size + 1;
		}
		return size;
	}
	PE("This type of variable ["+ToString(obj) + "] is not supported! Length()", "L0018");
	return -1;
}

/*Удаление части строки*/
string l_SubStr(sol::object Str_, int pos, int size) {
	string Str = ToString(Str_, "");
	if (Str == "") { PE("String cannot be empty! SubStr('" + Str + "')", "L0019"); return "ERROR_L0019"; }
	if (pos < 0) { PE("Position cannot be < 0! SubStr('" + Str + "'," + to_string(pos) + "," + to_string(size) + ")","L0021"); return "ERROR_L0021"; }
	if ((size) <= 0) { PE("Size cannot be <= 0! SubStr('" + Str + "'," + to_string(pos) + "," + to_string(size) + ")", "L0020"); return "ERROR_L0020"; }
	if (pos > Str.length()) { PW("Position ["+to_string(pos) + "] goes beyond the string ["+to_string(Str.length()) + "]! SubStr('" + Str + "'," + to_string(pos) + "," + to_string(size) + ")", "LW0021"); }
	if ((size + pos) > Str.length()) { PW("Size [" + to_string(size) + "] extends beyond the string ["+to_string(Str.length()) + "]! SubStr('" + Str + "'," + to_string(pos) + "," + to_string(size) + ")", "LW0020"); }
	return WStringToString(StringToWString(Str).erase(pos, size));
}

/*Превращает строку в число*/
float l_ToNumber(sol::object Str_, sol::object IfNil) {
	return ToFloat(Str_,ToFloat(IfNil,0));
}

/*Запуск команды cmd*/
string l_Cmd(sol::object command_) {
	string command = ToString(command_);
	if (SafeMode()) { PW("Function [Cmd('" + command + "')] cannot be started in SafeMode!", "LW0014"); return "ERROR_LW0014"; }
	else {
		return system_withresult(command);
	}
	return "";
}

/*Создать окно*/
void l_CreateWindow(sol::object id_, sol::object Title_, int sizex, int sizey) {
	string id = ToString(id_, EmptyWindow);
	string Title = ToString(Title_, EmptyWindow);
	int x = sizex;
	int y = sizey;
	if (sizex == 0) { x = 640; }
	if (sizey == 0) { y = 480; }
	CreateWindowGLFW(id, x, y, Title);
}

/*Уничтожить окно*/
void l_DestroyWindow(sol::object id) {
	DestroyWindowGLFW(ToString(id,EmptyWindow));
}

/*Проверяет есть ли окно в данных*/
bool l_HasWindow(sol::object id) {
	return HasWindow(ToString(id,EmptyWindow));
}

/*Делает окно главным*/
void l_SetWindowMain(sol::object id) {
	SetWindowMain(ToString(id,EmptyWindow));
}

/*Получить айди главного окна*/
string l_MainWindow() {
	return GetSessionInfo("MainWindow");
}

/*Получить сид*/
int l_Seed() {
	return StringToInt(GetSessionInfo("Seed"));
}

/*Установить сид*/
void l_SetSeed(int seed) {
	SetRandomSeed(seed);
}

/*Получить размер окна*/
l_Vector2 l_GetWindowSize(sol::object id) {
	return l_Vector2(GetWindowSize(ToString(id, EmptyWindow), false), GetWindowSize(ToString(id, EmptyWindow), true));
}

/*Изменить размер экрана*/
void l_SetWindowSize(sol::object id, l_Vector2 v) {
	SetWindowSize(ToString(id, EmptyWindow), false, v.x);
	SetWindowSize(ToString(id, EmptyWindow), true, v.y);
}

/*Изменяет название окна*/
void l_SetWindowTitle(sol::object id, sol::object title) {
	SetWindowTitle(ToString(id, EmptyWindow), ToString(title,"New Title"));
}

/*Изменение авторазмера у окна*/
void l_SetWindowAutoScale(sol::object id, sol::object b) {
	SetWindowAutosize(ToString(id, EmptyWindow), ToBool(b, true));
}

/*Синус ABS*/
float l_AbsSin(float f) {
	return abs(sin(f));
}

/*Косинус ABS*/
float l_AbsCos(float f) {
	return abs(cos(f));
}

/*(Синус+1)/2*/
float l_DSin(float f) {
	return (sin(f) + 1) / 2;
}

/*(Косинус+1)/2*/
float l_DCos(float f) {
	return (cos(f) + 1) / 2;
}

/*Запретить или размешить менять размер окна*/
void l_SetWindowResizable(sol::object id, sol::object b) {
	SetWindowResizable(ToString(id, EmptyWindow), ToBool(b,false));
}

/*Вызывает функцию когда окно закрывается*/
void l_SetWindowEventClosed(sol::object id_, sol::function func) {
	string id = ToString(id_, EmptyWindow);
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for SetWindowEventClosed('" + id + "')!", "L0013"); }
	else {
		SetWindowClosedEvent(id, func);
	}
}

/*Вызывает функцию когда игра закрывается*/
void l_SetEventClosed(sol::function func) {
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for SetEventClosed()!", "L0014"); }
	else {
		SetGameClosedEvent(func);
	}
}

/*Нажатие клавиши в окне*/
void l_SetWindowEventKeyPress(sol::object id, sol::function func) {
	SetWindowKPEvent(ToString(id, EmptyWindow), func);
}

/*Отжатие клавиши в окне*/
void l_SetWindowEventKeyRelease(sol::object id, sol::function func) {
	SetWindowKREvent(ToString(id, EmptyWindow), func);
}

/*Зажатие клавиши в окне*/
void l_SetWindowEventKeyHold(sol::object id, sol::function func) {
	SetWindowKHEvent(ToString(id, EmptyWindow), func);
}

/*Таблица зажатых клавиш*/
sol::table l_PressedKeys() {
	sol::table tbl = lua.create_table();
	for (const auto& p : GetPressedKeys()) {
		tbl.set(p.first,p.second);
	}
	return tbl;
}

/*Вернуть случайное число от 0 до 1 (Целое)*/
int l_RRandom(int min, int max) {
	if (min == 0 && max == 0) { return round(Random()); }
	return round(Random(min, max));
}

/*Установить позицию окна*/
void l_SetWindowPosition(sol::object id, l_Vector2 v) {
	SetWindowPosition(ToString(id, EmptyWindow), v.x, true);
	SetWindowPosition(ToString(id, EmptyWindow), v.x, false);
}

/*Получить позицию окна*/
l_Vector2 l_GetWindowPosition(sol::object id) {
	return l_Vector2(GetWindowPosition(ToString(id, EmptyWindow), true), GetWindowPosition(ToString(id, EmptyWindow), false));
}

/*Получает дробное число из числа*/
float l_Frac(float f) {
	return GetFractionalPart(f);
}

/*Разбивает значение с плавающей запятой на дробную и целую части*/
std::tuple<float, int> l_Modf(float f) {
	return std::make_tuple(l_Frac(f), l_Trunc(f));
}

/*Получить весь JSON файл в виде таблицы*/
sol::table l_JSONTable(sol::object path) {
	map<string, string> data = ReadAllJson(ToString(path));
	sol::table tbl = lua.create_table();
	for (const auto& p : data) {
		tbl.set(p.first, p.second);
	}
	return tbl;
}

/*Получить ключ и переменную из таблицы*/
void l_Pairs(sol::table table, sol::function func) {
	int i = 0;
	for (auto& v : table) {
		i++;
		StartFunction(func, {v.first,v.second,i});
	}
}

/*Получить строку из объекта*/
string l_ToString(sol::object obj) {
	return ToString(obj);
}

/*Получить тип переменной*/
string l_GetType(sol::object obj) {
	return sol::type_name(lua, obj.get_type());
}

/*Открыть ссылку*/
void l_OpenLink(sol::object html_) {
	string html = ToString(html_, "https://woowz11.github.io/woowzsite/woowzengine.html");
	if (SafeMode()) { PW("Function [OpenLink('" + html + "')] cannot be started in SafeMode!", "LW0019"); }
	else {
		if (CheckInternet()) {
			system(StringToConstChar("start \"\" " + html));
		}
		else {
			PW("Need inernet to use OpenLink('"+html+"')!","LW0029");
		}
	}
}

/*Получить время сколько уже активен движок*/
int l_ActiveTime() {
	return GetActiveTime();
}

/*Установить позицию камеры*/
void l_SetCameraPosition(sol::object scene_, l_Vector2 f) {
	string scene = ToString(scene_, EmptyScene);
	SetCameraPosition(scene, f.x, true);
	SetCameraPosition(scene, f.y, false);
}

/*Получить позицию камеры*/
l_Vector2 l_GetCameraPosition(sol::object scene_) {
	string scene = ToString(scene_, EmptyScene);
	return l_Vector2(GetCameraPosition(scene, true), GetCameraPosition(scene, false));
}

/*Нажатие клавиши на мышке в окне*/
void l_SetWindowEventMousePress(sol::object id, sol::function func) {
	SetWindowMPEvent(ToString(id,EmptyWindow), func);
}

/*Отжатие клавиши на мышке в окне*/
void l_SetWindowEventMouseRelease(sol::object id, sol::function func) {
	SetWindowMREvent(ToString(id,EmptyWindow), func);
}

/*Зажатие клавиши на мышке в окне*/
void l_SetWindowEventMouseHold(sol::object id, sol::function func) {
	SetWindowMHEvent(ToString(id,EmptyWindow), func);
}

/*Получить FPS*/
int l_GetFPS() {
	return GetFPS();
}

/*Создать сцену*/
void l_CreateScene(sol::object id) {
	CreateScene(ToString(id, EmptyScene));
}

/*Установить цвет заднего фона у сцены*/
void l_SetSceneBackgroundColor(sol::object id, sol::object color) {
	SetSceneBackgroundColor(ToString(id, EmptyScene), ObjToColor(color).ToCPP());
}

/*Установить сцену на окно*/
void l_SetSceneWindow(sol::object window, sol::object id) {
	SetWindowScene(ToString(window, EmptyWindow), ToString(id, EmptyScene));
}

/*Создать спрайт*/
void l_CreateSprite(sol::object id, sol::object sceneid) {
	CreateSprite(ToString(id, EmptySprite), ToString(sceneid, EmptyScene));
}

/*Установить позицию спрайта*/
void l_SetSpritePosition(sol::object sceneid, sol::object id, l_Vector2 position) {
	SetSpritePosition(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), position);
}

/*Установить цикл на рендер*/
void l_CycleRender(sol::function f) {
	if (f && f.valid()) {
		SetDTFunction(f);
	}
	else {
		PE("Function not found or does not exist for CycleRender()!","L0028");
	}
}

/*Установить масштаб для камеры*/
void l_SetCameraZoom(sol::object sceneid_, float f) {
	string sceneid = ToString(sceneid_);
	if (f == 0) {
		PE("Camera zoom cannot be 0! SetCameraZoom('"+sceneid+"',0)","L0023");
		SetCameraZoom(sceneid, 1);
	}
	else {
		SetCameraZoom(sceneid, f);
	}
}

/*Получить маштаб камеры*/
float l_GetCameraZoom(sol::object sceneid) {
	return GetCameraZoom(ToString(sceneid));
}

/*Получить позицию курсора*/
l_Vector2 l_GetMousePosition(sol::object windowid) {
	Vector2 v = GetMousePosition(ToString(windowid));
	return l_Vector2(v.x, v.y);
}

/*Экранные координаты в мировые*/
l_Vector2 l_ScreenToWorld(sol::object windowid, l_Vector2 screencord) {
	Vector2 v = ScreenToWorld(GetWindowByID(ToString(windowid)), Vector2(screencord.x, screencord.y));
	return l_Vector2(v.x, v.y);
}

/*Получить мировую позицию курсора*/
l_Vector2 l_GetMouseWPosition(sol::object windowid) {
	Vector2 v = ScreenToWorld(GetWindowByID(ToString(windowid)), GetMousePosition(ToString(windowid)));
	return l_Vector2(v.x, v.y);
}

/*Мировые координаты в экранные*/
l_Vector2 l_WorldToScreen(sol::object windowid, l_Vector2 worldcord) {
	Vector2 v = WorldToScreen(GetWindowByID(ToString(windowid)), Vector2(round(worldcord.x), round(worldcord.y)));
	return l_Vector2(v.x, v.y);
}

/*Получить знак числа, + или -*/
float l_Sign(float f) {
	float result = 1;
	if (f < 0) {
		result = -1;
	}
	return result;
}

/*Устанавливает название активности*/
void l_SetDiscordActivityTitle(sol::object s_) {
	string s = ToString(s_);
	if (s.length() == 1 || s.length() == 2) {
		PW("Title cannot contain 1,2 characters! Need less or more! SetDiscordActivityTitle('"+s+"')","LW0022");
	}
	else {
		SetDiscordActivityTitle(s);
	}
}

/*Устанавливает описание активности*/
void l_SetDiscordActivityDescription(sol::object s_) {
	string s = ToString(s_);
	if (s.length() == 1 || s.length() == 2) {
		PW("Description cannot contain 1,2 characters! Need less or more! SetDiscordActivityDescription('" + s + "')", "LW0023");
	}
	else {
		SetDiscordActivityDescription(s);
	}
}

/*Получить имя аккаунта windows*/
string l_GetUserName() {
	return GetUserName_();
}

/*Проверить есть подключение к интернету или нет*/
bool l_CheckInternet() {
	return CheckInternet();
}

/*Конвертирует строку в bool*/
bool l_ToBool(sol::object s, sol::object ifnil) {
	return ToBool(s,ToBool(ifnil,false));
}

/*Запускает строку кода lua*/
string l_RunLua(sol::object s, sol::object DontError) {
	string code = ToString(s, "");
	if (code == "") { PW("Empty code in RunLua()!","LW0024"); return "WARNING_LW0024"; }
	sol::protected_function_result result = lua.safe_script(code, &sol::script_pass_on_error);
	if (!result.valid()) {
		sol::error err = result.get<sol::error>();
		string what = err.what();
		if (!ToBool(DontError,false)) { PE(what, "GLUA"); }
		return what;
	}

	return "Succed";
}

/*Функция которая повториться определённое кол-во раз*/
void l_Repeat(sol::function func, int count, int milisec) {
	if (func == sol::nil || !func.valid()) { PE("Empty function! Repeat(nil,"+to_string(count) + ","+to_string(milisec) + ")", "L0024"); }
	else {
		if (count > 0) {
			if (milisec < 0) { PE("Expected time must be >= 0! Repeat(function," + to_string(count) + "," + to_string(milisec) + ")", "L0026"); }
			else {
				SetRepeatFunction(func, count, milisec);
			}
		}
		else {
			PE("Number of repetitions must be > 0! Repeat(function," + to_string(count) + "," + to_string(milisec) + ")", "L0025");
		}
	}
}

/*Проверяет запущено приложение или нет*/
bool l_ProgramLaunched(sol::object name) {
	return ProgramLaunched(ToString(name) + ".exe");
}

/*Проверяет запущен ли Discord*/
bool l_DiscordLaunched() {
	return DiscordLaunched();
}

/*Получить список запущеных программ*/
sol::table l_ProgramsLaunched() {
	map<int, string> programs = ProgramsLaunched();
	sol::table tbl = lua.create_table();
	for (const auto& pair : programs) {
		tbl.set(pair.first,pair.second);
	}
	return tbl;
}

/*Быстрая отправка сообщения в консоль*/
void l_PrintFast(sol::object str_, int color) {
	string str = ToString(str_);
	if (color < 0 || color>255) { PE("The color must match color <= 255 & color >= 0 in PrintFast('" + str + "'," + to_string(color) + ")!", "L0027"); }
	int color_ = color;
	if (color == 0) { color_ = 7; }
	if (color == 7) { color_ = 0; }
	PrintToConsole(str,color_);
}

/*Останавливает процесс*/
void l_StopProgram(sol::object name_) {
	string name = ToString(name_) + ".exe";
	if (SafeMode()) { PW("Function [StopProgram('" + name + "')] cannot be started in SafeMode!", "LW0025"); }
	else {
		if (ProgramLaunched(name)) {
			StopProgram(name);
		}
		else {
			PW("Program not found! StopProgram('"+name+"')", "LW0026");
		}
	}
}

/*Завершает работу компьютера*/
void l_ExitPC() {
	if (SafeMode()) { PW("Function [ExitPC()] cannot be started in SafeMode!", "LW0028"); }
	else {
		StopPC();
	}
}

/*Определяет находится ли точка за пределами окна*/
bool l_PointOutsideWindow(l_Vector2 point, sol::object windowid_) {
	string windowid = ToString(windowid_, EmptyWindow);
	return PointOutside_(point, windowid);
}

/*Получить список файлов в папке*/
sol::table l_GetFilesFromDirectory(sol::object dir_) {
	string dir = ToString(dir_,"C:\\Windows");
	map<string, string> files = GetFilesFromDirectory(dir);
	sol::table tbl = lua.create_table();
	for (const auto& pair : files) {
		tbl.set(pair.first, pair.second);
	}
	return tbl;
}

/*Получить название файла из пути*/
string l_GetFileName(sol::object dir) {
	return GetFileName(ToString(dir));
}

/*Получить тип файла*/
string l_GetFileType(sol::object dir_) {
	string dir = ToString(dir_);
	return GetFileType(dir);
}

/*Установить размер спрайта*/
void l_SetSpriteSize(sol::object sceneid_, sol::object id_, l_Vector2 size) {
	string sceneid = ToString(sceneid_, EmptyScene);
	string id = ToString(id_, EmptySprite);
	if (size.x <= 0 || size.y <= 0) {
		PW("Sprite size cannot be <= 0 SetSpriteSize('"+sceneid+"','"+id+"',Vector2("+to_string(size.x) + ","+to_string(size.y) + "))", "LW0034");
	}
	else {
		SetSpriteSize(sceneid, id, size);
	}
}

/*Установить позицию угла*/
void l_SetSpriteCorner(sol::object sceneid, sol::object id, sol::object left, sol::object top, l_Vector2 pos) {
	SetSpriteCorner(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), pos, ToBool(left,false), ToBool(top,false));
}

/*Установить лимит фпс*/
void l_SetFPSTarget(int fpstarget) {
	SetTargetFPS(fpstarget);
}

/*Вызывает функцию каждый раз когда отправляется сообщение*/
void l_SetEventPrint(sol::function f) {
	if (f.valid()) {
		SetPrintFunction(f);
	}
}

/*Установить прозрачность окна*/
void l_SetWindowTransparency(sol::object window, int i) {
	string w = ToString(window, EmptyWindow);
	if (i < 0 || i>255) {
		PE("Transparency cannot be < 0 or > 255! SetWindowTransparency('"+w+"'," + to_string(i) + ")", "L0029");
	}
	else {
		if (SafeMode() && i < 50) { PW("Function [SetWindowTransparency("+to_string(i) + ")] cannot be started in SafeMode!", "LW0028"); }
		else {
			SetWindowTransparency(w, i);
		}
	}
}

/*Получить прозрачность окна*/
int l_GetWindowTransparency(sol::object window) {
	return GetWindowTransparency(ToString(window));
}

/*Открыть файл*/
void l_OpenFile(sol::object path_) {
	string path = StringToPath(ToString(path_));
	if (SafeMode()) { PW("Function [OpenFile('" + path + "')] cannot be started in SafeMode!", "LW0030"); }
	else {
		if (HasDirectory(path)) {
			OpenFile(path);
		}
		else {
			PE("File not found! OpenFile('"+path+"')", "L0030");
		}
	}
}

/*Установить цвет спрайту*/
void l_SetSpriteColor(sol::object sceneid, sol::object id, l_Color color) {
	SetSpriteColor(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), color);
}

/*Получить цвет спрайта*/
l_Color l_GetSpriteColor(sol::object sceneid, sol::object id) {
	return GetSpriteColor(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Установить слой текстуре*/
void l_SetSpriteLayer(sol::object sceneid, sol::object id, float zindex) {
	SetSpriteLayer(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), zindex);
}

/*Получить слой текстуры*/
float l_GetSpriteLayer(sol::object sceneid, sol::object id) {
	return GetSpriteLayer(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Вернуть случайное число от 0 до 1 (без сохранения сида)*/
float l_FRandom(float min, float max) {
	if (min == 0 && max == 0) { return Random(true); }
	return Random(min, max, true);
}

/*Вернуть случайное число от 0 до 1 (Целое) (без сохранения сида)*/
int l_FRRandom(int min, int max) {
	if (min == 0 && max == 0) { return round(Random(true)); }
	return round(Random(min, max,true));
}

/*Получить список спрайтов на сцене*/
sol::table l_GetSprites(sol::object sceneid) {
	vector<string> sprites = GetSpritesOnScene(ToString(sceneid, EmptyScene));
	sol::table tbl = lua.create_table();
	for (const auto& sprite : sprites) {
		tbl.add(sprite);
	}
	return tbl;
}

/*Запускает функцию в новом потоке*/
void l_NoWait(sol::function func) {
	InOtherThread(func);
}

/*Получает установленый язык в системе*/
string l_GetSystemLanguage() {
	return GetSystemLanguage();
}

/*Получает выбраный язык*/
string l_GetLanguage() {
	return GetLanguage();
}

/*Проверяет есть ли такой спрайт на сцене*/
bool l_HasSprite(sol::object sceneid, sol::object id) {
	return HasSprite(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Устанавливает текстуру спрайту*/
void l_SetSpriteTexture(sol::object sceneid, sol::object id, sol::object texture) {
	SetSpriteTexture(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), ToString(texture,EmptyTexture));
}

/*Устанавливает размер как у текстуры*/
void l_SetSpriteSizeByTexture(sol::object sceneid, sol::object id, float sizeextra) {
	if (sizeextra == 0) { sizeextra = 1; }
	SetSpriteSizeByTexture(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), 1/sizeextra);
}

/*Получить размер текстуры*/
l_Vector2 l_GetTextureSize(sol::object texture) {
	return GetTextureSize(ToString(texture,EmptyTexture));
}

/*Получить размер спрайта*/
l_Vector2 l_GetSpriteSize(sol::object sceneid, sol::object id) {
	return GetSpriteSize(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Получить позицию спрайта*/
l_Vector2 l_GetSpritePosition(sol::object sceneid, sol::object id) {
	return GetSpritePosition(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Отеркалить спрайт по X*/
void l_SetSpriteMirrorX(sol::object sceneid, sol::object id, sol::object b) {
	SetSpriteMirror(ToString(sceneid, EmptyScene), ToString(id, EmptySprite),true,ToBool(b,true));
}

/*Отеркалить спрайт по Y*/
void l_SetSpriteMirrorY(sol::object sceneid, sol::object id, sol::object b) {
	SetSpriteMirror(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), false, ToBool(b,true));
}

/*Получить отзеркаливание по X*/
bool l_GetSpriteMirrorX(sol::object sceneid, sol::object id) {
	return GetSpriteMirror(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), true);
}

/*Получить отзеркаливание по Y*/
bool l_GetSpriteMirrorY(sol::object sceneid, sol::object id) {
	return GetSpriteMirror(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), false);
}

/*Установить позицию угла UV*/
void l_SetSpriteUVCorner(sol::object sceneid, sol::object id, sol::object left, sol::object top, l_Vector2 pos) {
	SetSpriteCornerUV(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), pos, ToBool(left,false), ToBool(top,false));
}

/*Установить поворот спрайта*/
void l_SetSpriteOrientation(sol::object sceneid, sol::object id, float deg) {
	SetSpriteOrientation(ToString(sceneid, EmptyScene), ToString(id, EmptySprite),deg);
}

/*Получить поворот спрайта*/
float l_GetSpriteRotation(sol::object sceneid, sol::object id) {
	return GetSpriteRotation(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Берёт число между двумя числами по t*/
float l_Lerp(float a, float b, float t) {
	if (t < 0 || t > 1) {
		PW("The interpolation value cannot be < 0 or > 1! Lerp("+to_string(a) + ","+to_string(b) + ","+to_string(t) + ")", "LW0033");
		return -1;
	}
	else {
		return lerp(a, b, t);
	}
}

/*Градусы в радианы*/
float l_ToRad(float deg) {
	return DegToRad(deg);
}

/*Радианы в градусы*/
float l_ToDeg(float rad) {
	return RadToDeg(rad);
}

/*Установить центр спрайта*/
void l_SetSpriteCenter(sol::object sceneid, sol::object id, l_Vector2 pos) {
	SetSpriteCenter(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), pos);
}

/*Получить центр спрайта*/
l_Vector2 l_GetSpriteCenter(sol::object sceneid, sol::object id) {
	return GetSpriteCenter(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Получить список окон*/
sol::table l_GetWindows() {
	vector<string> windowslist = GetWindows();
	sol::table tbl = lua.create_table();
	for (const auto& sprite : windowslist) {
		tbl.add(sprite);
	}
	return tbl;
}

/*Получить окно по сцене*/
string l_GetWindow(sol::object sceneid) {
	return GetWindowByScene(ToString(sceneid, EmptyScene));
}

/*Получить сцену по окну*/
string l_GetScene(sol::object windowid) {
	return GetSceneByWindow(ToString(windowid, EmptyWindow));
}

/*Установить фотографию на рабочий стол*/
void l_SetDesktopBackground(sol::object path_) {
	string path = StringToPath(ToString(path_, EmptyImage));
	if (SafeMode()) {
		PW("Function [SetDesktopBackground('" + path + "')] cannot be started in SafeMode!", "LW0031");
	}
	else {
		if (HasDirectory(path)) {
			SetDesktopBackground(path);
		}
		else {
			PE("File not found! SetDesktopBackground('" + path + "')","L0031");
		}
	}
}

/*Получить громкость компьютера*/
int l_GetVolume() {
	return GetVolume();
}

/*Установить громкость компьютера*/
void l_SetVolume(int v) {
	if (SafeMode()) {
		PW("Function [SetVolume(" + to_string(v) + ")] cannot be started in SafeMode!", "LW0032");
	}
	else {
		if (v > 100 || v < 0) {
			PE("Volume must be > 0 and < 100! SetVolume("+to_string(v) + ")", "L0032");
		}
		else {
			SetVolume(v);
		}
	}
}

/*Создать текстуру*/
void l_LoadTexture(sol::object id_, sol::object path_, sol::object savecolors) {
	string path = StringToPath(ToString(path_));
	string id = ToString(id_, EmptyTexture);
	if (HasDirectory(path)) {
		CreateTexture(id, path, ToBool(savecolors,false));
	}
	else {
		PE("File not found! LoadTexture('"+id+"','"+path+"')","L0033");
	}
}

/*Установить видимость спрайта*/
void l_SetSpriteVisible(sol::object sceneid, sol::object id, sol::object visible) {
	SetSpriteVisible(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), ToBool(visible,false));
}

/*Получить видимость спрайта*/
bool l_GetSpriteVisible(sol::object sceneid, sol::object id) {
	return GetSpriteVisible(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Установить размытие текстуре*/
void l_SetTextureBlur(sol::object id, sol::object blur) {
	SetTextureBlur(ToString(id, EmptyTexture), ToBool(blur,false));
}

/*Получить массив цветов в текстуре*/
sol::table l_GetTextureColors(sol::object id) {
	vector<l_Color> colors = GetTextureColors(ToString(id, EmptyTexture));
	sol::table tbl = lua.create_table();
	for (const auto& color : colors) {
		tbl.add(color);
	}
	return tbl;
}

/*Установить название консоли*/
void l_SetConsoleTitle(sol::object title) {
	SetConsoleTitle_(ToString(title, "New Console Title"));
}

/*Создать текстуру*/
void l_CreateTexture(sol::object id_, int sizex, int sizey, sol::table colors, sol::object savecolors) {
	string id = ToString(id_, EmptyTexture);
	if (sizex <= 0 || sizey <= 0) {
		PE("Size of the created texture cannot be <= 0 CreateTexture('"+id+"',"+to_string(sizex) + ","+to_string(sizey) + ")", "L0034");
	}
	else {
		if (sizex * sizey != colors.size()) {
			PW("Size of the array with colors does not match the size of the created texture. "+to_string(colors.size()) + " ~= " + to_string(sizex * sizey) + " CreateTexture('" + id + "'," + to_string(sizex) + "," + to_string(sizey) + ")", "LW0035");
		}
		vector<l_Color> colors_result;
		int size = colors.size();
		for (size_t i = 1; i <= size; i++) {
			colors_result.push_back(ObjToColor(colors[i]));
		}
		CreateTextureByArray(id, sizex, sizey, colors_result, ToBool(savecolors,false));
	}
}

/*Записывает данные в изображение файл*/
void l_WriteImage(sol::object path_,int sizex, int sizey, sol::table colors) {
	string path = ToString(path_);
	if (SafeMode()) { PW("Function [WriteImage('" + path + "'," + to_string(sizex) + ","+to_string(sizey) + ")] cannot be started in SafeMode!", "LW0036"); }
	else {
		if (HasDirectory(path)) {
			if (sizex <= 0 || sizey <= 0) {
				PE("Size of the created texture cannot be <= 0 WriteImage('" + path + "'," + to_string(sizex) + "," + to_string(sizey) + ")", "L0036");
			}
			else {
				if (sizex * sizey != colors.size()) {
					PW("Size of the array with colors does not match the size of the created texture. " + to_string(colors.size()) + " ~= " + to_string(sizex * sizey) + " WriteImage('" + path + "'," + to_string(sizex) + "," + to_string(sizey) + ")", "LW0037");
				}
				vector<l_Color> colors_result;
				int size = colors.size();
				for (size_t i = 1; i <= size; i++) {
					colors_result.push_back(ObjToColor(colors[i]));
				}
				WriteImage(path, sizex, sizey, colors_result);
			}
		}
		else {
			PE("File not found! WriteImage('" + path + "'," + to_string(sizex) + "," + to_string(sizey) + ")","L0035");
		}
	}
}

/*Загрузка шейдера в спрайт*/
void l_SetSpriteShader(sol::object sceneid, sol::object id, sol::object shaderid) {
	SetSpriteShader(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), ToString(shaderid, EmptyShader));
}

/*Установить высоту спрайта*/
void l_SetSpriteHeight(sol::object sceneid, sol::object id, float height) {
	SetSpriteHeight(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), height);
}

/*Проверяет чётное число или нет*/
bool l_Even(float f, float f2) {
	if (f2 == 0) { f2 = 2; }
	float f_ = f / f2;
	return GetFractionalPart(f_) == 0;
}

/*Возводит в степень f^f2*/
float l_Pow(float f, float f2) {
	if (f2 == 0 && f == 0) {
		return 1;
	}
	else {
		if(f2<0 && f == 0){
			PW("Base number at negative degree cannot be = 0! Pow(" + to_string(f) + "," + to_string(f2) + ")","LW0039");
			return 0;
		}
		else {
			if ((f2<1&&f2>-1) && l_Even(1/f2,2) && f < 0){
				PE("It is not possible to get the even root of a negative number! Pow("+to_string(f) + ","+to_string(f2) + ")", "L0041");
				return 0;
			}
			else {
				return pow(f, f2);
			}
		}
	}
}

/*Удалить файл*/
void l_RemoveFile(sol::object pathandname_) {
	string pathandname = ToString(pathandname_);
	if (SafeMode()) { PW("Function [RemoveFile('" + pathandname + "')] cannot be started in SafeMode!", "LW0040"); }
	else {
		bool has = HasDirectory(pathandname);
		if (has) {
			RemoveFile(pathandname);
		}
		else {
			PE("File not found! RemoveFile('"+pathandname+"')", "L0037");
		}
	}
}

/*Воспроизводит дефолтный звук Windows*/
void l_PlayBeep() {
	PlayBeep();
}

/*Получить шейдер спрайта*/
string l_GetSpriteShader(sol::object sceneid, sol::object id) {
	return GetSpriteShader(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Получить высоту спрайта*/
float l_GetSpriteHeight(sol::object sceneid, sol::object id) {
	return GetSpriteHeight(ToString(sceneid, EmptyScene), ToString(id, EmptySprite));
}

/*Получить данные пользователя Discord*/
void l_GetDiscordUserInfo(sol::object userid_, sol::function func) {
	string userid = ToString(userid_, EmptyDUser);
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for GetDiscordUserInfo('" + userid + "')!", "L0038"); }
	else {
		GetDiscordUserInfo(userid, func);
	}
}

/*Получить айди текущего пользователя*/
string l_GetDiscordCurrentUser() {
	return GetDiscordCurrentUser();
}

/*Создаёт текст*/
void l_CreateText(sol::object id_, sol::object sceneid_, sol::object text_) {
	string id = ToString(id_,EmptyText);
	string sceneid = ToString(sceneid_,EmptyScene);
	string text = ToString(text_, "Default text!");
	CreateText(id,sceneid,text);
}

/*Создаёт шрифт*/
void l_CreateFont(sol::object id_, sol::object path_) {
	string id = ToString(id_, EmptyFont);
	string path = ToString(path_);
	CreateFont(id,path);
}

/*Установить позицию тексту*/
void l_SetTextPosition(sol::object sceneid_, sol::object id_, l_Vector2 pos) {
	SetTextPosition(ToString(sceneid_,EmptyScene),ToString(id_,EmptyText),pos);
}

/*Установить шрифт тексту*/
void l_SetTextFont(sol::object sceneid_, sol::object id_, sol::object font) {
	SetTextFont(ToString(sceneid_, EmptyScene), ToString(id_, EmptyText),ToString(font,"default"));
}

/*Установить цвет тексту*/
void l_SetTextColor(sol::object sceneid, sol::object id, l_Color color) {
	SetTextColor(ToString(sceneid, EmptyScene), ToString(id, EmptyText), color);
}

/*Получить символ ASCII*/
char l_GetASCIIChar(int i) {
	return static_cast<char>(i);
}

/*Установить текст тексту*/
void l_SetTextText(sol::object sceneid, sol::object id, sol::object text) {
	SetTextText(ToString(sceneid, EmptyScene), ToString(id, EmptyText), ToString(text,"New Text!"));
}

/*Установить высоту тексту*/
void l_SetTextHeight(sol::object sceneid, sol::object id, float height) {
	SetTextHeight(ToString(sceneid, EmptyScene), ToString(id, EmptyText), height);
}

/*Превратить строку в масив цветов*/
sol::table l_StringToColors(sol::object str_, sol::table colors, sol::object errorcolor_) {
	l_Color errorcolor = ObjToColor(errorcolor_,ErrorColor);
	string str = ToString(str_, "0");
	sol::table tbl = lua.create_table();
	for (size_t i = 0; i < str.length(); i++) {
		char c_ = str[i];
		string c = {c_};
		if (colors[c]!=sol::nil) {
			tbl.add(colors[c]);
		}
		else {
			tbl.add(errorcolor);
		}
	}
	return tbl;
}

/*Показывать или спрятать курсор*/
void l_ShowCursor(sol::object id, sol::object b) {
	ShowCursor_(ToString(id, EmptyWindow),ToBool(b,true));
}

/*Установить моноспейс текст*/
void l_SetTextMono(sol::object sceneid, sol::object id, sol::object b) {
	SetTextMono(ToString(sceneid, EmptyScene), ToString(id, EmptyText), ToBool(b,true));
}

/*Получить показывать или спрятать курсор*/
bool l_GetShowCursor(sol::object id) {
	return GetShowCursor(ToString(id, EmptyWindow));
}

/*Получить моноспейс текст*/
bool l_GetTextMono(sol::object sceneid, sol::object id) {
	return GetTextMono(ToString(sceneid, EmptyScene), ToString(id, EmptyText));
}

/*Получить текст тексту*/
string l_GetTextText(sol::object sceneid, sol::object id) {
	return GetTextText(ToString(sceneid, EmptyScene), ToString(id, EmptyText));
}

/*Получить высоту тексту*/
float l_GetTextHeight(sol::object sceneid, sol::object id) {
	return GetTextHeight(ToString(sceneid, EmptyScene), ToString(id, EmptyText));
}

/*Получить позицию тексту*/
l_Vector2 l_GetTextPosition(sol::object sceneid_, sol::object id_) {
	return GetTextPosition(ToString(sceneid_, EmptyScene), ToString(id_, EmptyText));
}

/*Получить шрифт тексту*/
string l_GetTextFont(sol::object sceneid_, sol::object id_) {
	return GetTextFont(ToString(sceneid_, EmptyScene), ToString(id_, EmptyText));
}

/*Получить цвет тексту*/
l_Color l_GetTextColor(sol::object sceneid, sol::object id) {
	return GetTextColor(ToString(sceneid, EmptyScene), ToString(id, EmptyText));
}

/*Получить позицию левого верхнего угла*/
l_Vector2 l_GetSpriteCorner(sol::object sceneid, sol::object id, sol::object left, sol::object top) {
	return GetSpriteCorner(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), ToBool(left,false), ToBool(top,false));
}

/*Получить позицию угла UV*/
l_Vector2 l_GetSpriteUVCorner(sol::object sceneid, sol::object id, sol::object left, sol::object top) {
	return GetSpriteCornerUV(ToString(sceneid, EmptyScene), ToString(id, EmptySprite), ToBool(left,false), ToBool(top,false));
}

/*Получить символ из юникод таблицы*/
string l_GetUnicodeChar(int i) {
	wchar_t c(i);
	return WStringToString(WCharToWString(c));
}

/*Создаёт message box*/
void l_MessageBox(sol::object title, sol::object text, sol::object messageboxtype, sol::object messageboxbuttons, sol::object blockprogram, sol::function button1, sol::function button2, sol::function button3) {
	if (SafeMode()) {
		PW("Function [MessageBox('" + ToString(title, "New Message Box") + "','" + ToString(text, "Hello world!") + "','" + ToString(messageboxtype,"") + "')] cannot be started in SafeMode!", "LW0042");
	}
	else {
		long icon = GetFromMap(EnumMessageBoxType, ToString(messageboxtype, ""));
		long buttons = GetFromMap(EnumMessageBoxButtons, ToString(messageboxbuttons, ""));
		int result = MessageBoxW(NULL, StringToLPCWSTR(ToString(text, "Hello world!")), ConstCharToConstWChar(StringToConstChar(ToString(title,"New Message Box"))), icon | buttons | (ToBool(blockprogram,false)?MB_TASKMODAL:MB_APPLMODAL));
		if (buttons == MB_OK) {
			if (result == IDOK) {
				button1();
			}
		}
		else if (buttons == MB_ABORTRETRYIGNORE) {
			if (result == IDABORT) {
				button1();
			}
			else if (result == IDRETRY) {
				button2();
			}
			else if (result == IDIGNORE) {
				button3();
			}
		}
		else if (buttons == MB_CANCELTRYCONTINUE) {
			if (result == IDCANCEL) {
				button1();
			}
			else if (result == IDTRYAGAIN) {
				button2();
			}
			else if (result == IDCONTINUE) {
				button3();
			}
		}
		else if (buttons == MB_OKCANCEL) {
			if (result == IDOK) {
				button1();
			}
			else if (result == IDCANCEL) {
				button2();
			}
		}
		else if (buttons == MB_RETRYCANCEL) {
			if (result == IDRETRY) {
				button1();
			}
			else if (result == IDCANCEL) {
				button2();
			}
		}
		else if (buttons == MB_YESNO) {
			if (result == IDYES) {
				button1();
			}
			else if (result == IDNO) {
				button2();
			}
		}
		else if (buttons == MB_YESNOCANCEL) {
			if (result == IDYES) {
				button1();
			}
			else if (result == IDNO) {
				button2();
			}
			else if (result == IDCANCEL) {
				button3();
			}
		}
	}
}

/*Получить символ из woowzengine таблицы*/
string l_GetEngineChar(int i) {
	return GetEngineChar(i);
}

/*Делает окно для ImGui*/
void l_SetWindowImGui(sol::object id) {
	SetWindowImGui(ToString(id, EmptyWindow));
}

/*Создать окно в ImGui*/
void l_CreateImGuiWindow(sol::object id, sol::object title, sol::object visible) {
	CreateImGuiWindow(ToString(id,EmptyImGuiWindow),ToString(title,"New ImGui"),ToBool(visible,true));
}

/*Создать элемент в окне ImGui*/
void l_CreateImGuiElement(sol::object id, sol::object windowid, sol::object type) {
	CreateImGuiElement(ToString(id,EmptyImGuiElement),ToString(windowid,EmptyImGuiWindow),ToString(type,"Text"));
}

/*Изменить текст элемента ImGui*/
void l_SetImGuiElementText(sol::object id, sol::object text) {
	SetImGuiElementText(ToString(id,EmptyImGuiElement),ToString(text,"New Text"));
}

/*Изменить цвет элемента ImGui*/
void l_SetImGuiElementColor(sol::object id, sol::object color) {
	SetImGuiElementColor(ToString(id, EmptyImGuiElement),ObjToColor(color,ErrorColor));
}

/*Возвращает случайный цвет*/
l_Color l_RandomColor(sol::object WithAlpha) {
	return l_Color(l_FRRandom(0, 255), l_FRRandom(0, 255), l_FRRandom(0, 255), (ToBool(WithAlpha,false) ? l_FRRandom(0, 255) : 255));
}

/*Изменить ивент элемента ImGui*/
void l_SetImGuiElementEvent(sol::object id_, sol::function func) {
	string id = ToString(id_, EmptyImGuiElement);
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for SetImGuiElementEvent('" + id + "')!", "L0046"); }
	else {
		SetImGuiElementEvent(id, func);
	}
}

/*Добавить точку к элементу ImGui*/
void l_SetImGuiElementPoint(sol::object id, sol::object b) {
	SetImGuiElementPoint(ToString(id, EmptyImGuiElement), ToBool(b,true));
}

/*Добавить переменную (строку) к ImGui*/
void l_SetImGuiElementStringValue(sol::object id, sol::object str) {
	SetImGuiElementStringValue(ToString(id, EmptyImGuiElement), ToString(str, "String value!"));
}

/*Заставить элемент присоеденится к прошлому ImGui*/
void l_SetImGuiElementConnect(sol::object id, sol::object b) {
	SetImGuiElementConnect(ToString(id, EmptyImGuiElement), ToBool(b,true));
}

/*Добавить подсказку к ImGui*/
void l_SetImGuiElementTooltip(sol::object id, sol::object str) {
	SetImGuiElementTooltip(ToString(id, EmptyImGuiElement), ToString(str, "New Tooltip"));
}

/*Изменить размер шрифта ImGui*/
void l_SetImGuiFontSize(float f) {
	if (f > 0) {
		SetImGuiFontSize(f);
	}
	else {
		PE("Font scale cannot be <= 0! SetImGuiFontScale(" + to_string(f) + ")", "L0047");
	}
}

/*Отображать окно ImGui*/
void l_SetImGuiWindowVisible(sol::object id, bool b) {
	SetImGuiWindowActive(ToString(id, EmptyImGuiWindow), b);
}

/*Изменить переменную ImGui*/
void l_SetImGuiElementValue(sol::object id, sol::object v1, sol::object v2, sol::object v3, sol::object v4) {
	vector<variant<string, double, bool, l_Color>> val1 = {ToString(v1,"nil"),ToNumber(v1),ToBool(v1,false),ObjToColor(v1,ErrorColor,true)};
	vector<variant<string, double, bool, l_Color>> val2 = { ToString(v2,"nil"),ToNumber(v2),ToBool(v2,false),ObjToColor(v2,ErrorColor,true) };
	vector<variant<string, double, bool, l_Color>> val3 = { ToString(v3,"nil"),ToNumber(v3),ToBool(v3,false),ObjToColor(v3,ErrorColor,true) };
	vector<variant<string, double, bool, l_Color>> val4 = { ToString(v4,"nil"),ToNumber(v4),ToBool(v4,false),ObjToColor(v4,ErrorColor,true) };
	SetImGuiElementValue(ToString(id, EmptyImGuiElement), val1, val2, val3, val4);
}

/*Получить переменную ImGui*/
tuple<sol::object, sol::object, sol::object, sol::object> l_GetImGuiElementValue(sol::object id) {
	variant<string, vector<string>, double, vector<double>, bool, l_Color> getted = GetImGuiElementValue(ToString(id, EmptyImGuiElement));
	auto val = visit([](const auto& value) -> std::any { return value; }, getted);
	sol::object obj1 = sol::nil;
	sol::object obj2 = sol::nil;
	sol::object obj3 = sol::nil;
	sol::object obj4 = sol::nil;
	if (val.type() == typeid(vector<double>)) {
		vector<double> vs = std::any_cast<vector<double>>(val);
		obj1 = AnyToObject(vs[0]);
		if (vs.size() >= 2) {
			obj2 = AnyToObject(vs[1]);
			if (vs.size() >= 3) {
				obj3 = AnyToObject(vs[2]);
				if (vs.size() >= 4) {
					obj4 = AnyToObject(vs[3]);
				}
			}
		}
	}
	if (val.type() == typeid(vector<string>)) {
		vector<string> vs = std::any_cast<vector<string>>(val);
		obj1 = AnyToObject(vs[0]);
		if (vs.size() >= 2) {
			obj2 = AnyToObject(vs[1]);
			if (vs.size() >= 3) {
				obj3 = AnyToObject(vs[2]);
				if (vs.size() >= 4) {
					obj4 = AnyToObject(vs[3]);
				}
			}
		}
	}
	else {
		obj1 = AnyToObject(val);
	}

	return make_tuple(obj1, obj2, obj3, obj4);
}

string l_RemoveMagicalNumber(float f) {
	int celaya = truncf(f);
	string drob = to_string(abs(GetFractionalPart(f)));
	double r = (double)celaya + stod((drob.size() > 6) ? drob.substr(0, 6) : drob) * GetNumberZnak(celaya);
	std::stringstream stream;
	stream << std::fixed << std::setprecision(5) << r;
	drob = stream.str();
	size_t dotPos = drob.find('.');
	if (dotPos != std::string::npos) {
		drob.erase(drob.find_last_not_of('0') + 1, std::string::npos);
	}
	return drob;
}

/*Зона woowzengine*/

l_Color ObjToColor(sol::object obj, l_Color ifnil, bool ingoreerror) {
	string type = GetObjectType(obj);
	if (type == "color") {
		return obj.as<sol::userdata>().as<l_Color>();
	}
	else if (type == "nil") {
		return ifnil;
	}
	else {
		if (!ingoreerror) {
			PE("Failed to convert the object [" + ToString(obj) + "] to color!", "L0015");
		}
		return ErrorColor;
	}
}

string l_WIP() {
	PW("Called function has not been created!","LW0038");
	return "LW0038";
}

/*Функция для теста других функций (ЧИСТО ДЛЯ WOOWZ11)*/
void l_TestFunction(string winid) {
	if (StringToBool(GetSettingsInfo("Console")) && !SafeMode()) {
		
		

	}
	else {
		PW("This function is a test function! It can be called only when the console is enabled and safe mode disabled, also it may change every version of the engine, it cannot be used. TestFunction()", "LW0041");
	}
}

string GetObjectType(sol::object obj) {
	sol::type type = obj.get_type();

	switch (type) {
	case sol::type::string:
		return "string";
		break;
	case sol::type::number:
		return "number";
		break;
	case sol::type::nil:
		return "nil";
		break;
	case sol::type::boolean:
		return "bool";
		break;
	}

	if (type == sol::type::userdata) {
		sol::userdata u = obj.as<sol::userdata>();
		if (u.is<l_Vector2>()) {
			return "vector2";
		}
		else if (u.is<l_Vector3>()) {
			return "vector3";
		}
		else if (u.is<l_Vector4>()) {
			return "vector4";
		}
		else if (u.is<l_Color>()) {
			return "color";
		}
		else if (u.is<l_Vertex>()) {
			return "vertex";
		}
	}

	return "unknown";
}

float ToNumber(sol::object obj) {
	return obj.as<float>();
}

bool ToBool(sol::object obj, bool IfNil) {
	string type = GetObjectType(obj);
	sol::userdata u = obj.as<sol::userdata>();

	if (type == "string") {
		return StringToBool(obj.as<std::string>());
	}
	else if (type == "number") {
		return obj.as<double>() == 1;
	}
	else if (type == "nil") {
		return IfNil;
	}
	else if (type == "bool") {
		if (obj.as<bool>()) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

float ToFloat(sol::object obj, float IfNil) {
	string type = GetObjectType(obj);
	sol::userdata u = obj.as<sol::userdata>();

	if (type == "string") {
		return StringToFloat(obj.as<std::string>());
	}
	else if (type == "number") {
		return obj.as<double>();
	}
	else if (type == "nil") {
		return IfNil;
	}
	else if (type == "bool") {
		if (obj.as<bool>()) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

string ToString(sol::object obj, string IfNil) {
	string type = GetObjectType(obj);
	sol::userdata u = obj.as<sol::userdata>();

	if (type == "string") {
		return obj.as<std::string>();
	}
	else if (type == "number") {
		return DoubleToString(obj.as<double>());
	}
	else if (type == "nil") {
		return IfNil;
	}
	else if (type == "bool") {
		if (obj.as<bool>()) {
			return "true";
		}
		else {
			return "false";
		}
	}
	else if (type == "vector2") {
		l_Vector2 v2 = u.as<l_Vector2>();
		return "Vector2(" + DoubleToString(v2.x) + "," + DoubleToString(v2.y) + ")";
	}
	else if (type == "vector3") {
		l_Vector3 v3 = u.as<l_Vector3>();
		return "Vector3(" + DoubleToString(v3.x) + "," + DoubleToString(v3.y) + "," + DoubleToString(v3.z) + ")";
	}
	else if (type == "vector4") {
		l_Vector4 v4 = u.as<l_Vector4>();
		return "Vector4(" + DoubleToString(v4.x) + "," + DoubleToString(v4.y) + "," + DoubleToString(v4.z) + "," + DoubleToString(v4.w) + ")";
	}
	else if (type == "color") {
		l_Color c = u.as<l_Color>();
		return "Color(" + DoubleToString(c.r) + "," + DoubleToString(c.g) + "," + DoubleToString(c.b) + "," + DoubleToString(c.a) + ")";
	}
	else if (type == "vertex") {
		l_Vertex v = u.as<l_Vertex>();
		return "Vertex(Vector2(" + DoubleToString(v.Position.x) + "," + DoubleToString(v.Position.y) + "),Color(" + DoubleToString(v.Color.r) + "," + DoubleToString(v.Color.g) + "," + DoubleToString(v.Color.b) + "," + DoubleToString(v.Color.a) + "))";
	}

	return "Value type [" + sol::type_name(lua, obj.get_type()) + "]";
}

sol::object AnyToObject(any obj) {
	if (obj.type() == typeid(sol::object)) {
		return any_cast<const sol::object&>(obj);
	}
	else if (obj.type() == typeid(std::string)) {
		return sol::make_object(lua, std::any_cast<std::string>(obj));
	}
	else if (obj.type() == typeid(int)) {
		return sol::make_object(lua, std::any_cast<int>(obj));
	}
	else if (obj.type() == typeid(double)) {
		return sol::make_object(lua, std::any_cast<double>(obj));
	}
	else if (obj.type() == typeid(float)) {
		return sol::make_object(lua, std::any_cast<float>(obj));
	}
	else if (obj.type() == typeid(bool)) {
		return sol::make_object(lua, std::any_cast<bool>(obj));
	}
	else if (obj.type() == typeid(l_Color)) {
		return sol::make_object(lua, std::any_cast<l_Color>(obj));
	}
	return sol::nil;
}

void StartFunction(sol::function func, list<any> params) {
	if (func != sol::nil && func.valid()) {
		try {
			sol::object s0 = sol::nil;
			sol::object s1 = sol::nil;
			sol::object s2 = sol::nil;
			sol::object s3 = sol::nil;
			sol::object s4 = sol::nil;
			sol::object s5 = sol::nil;
			sol::object s6 = sol::nil;
			sol::object s7 = sol::nil;
			sol::object s8 = sol::nil;
			sol::object s9 = sol::nil;
			for (auto it = params.begin(); it != params.end(); ++it) {
				int index = std::distance(params.begin(), it);
				switch (index) {
				case 0:
					s0 = AnyToObject(*it);
					break;
				case 1:
					s1 = AnyToObject(*it);
					break;
				case 2:
					s2 = AnyToObject(*it);
					break;
				case 3:
					s3 = AnyToObject(*it);
					break;
				case 4:
					s4 = AnyToObject(*it);
					break;
				case 5:
					s5 = AnyToObject(*it);
					break;
				case 6:
					s6 = AnyToObject(*it);
					break;
				case 7:
					s7 = AnyToObject(*it);
					break;
				case 8:
					s8 = AnyToObject(*it);
					break;
				case 9:
					s9 = AnyToObject(*it);
					break;
				};
			}
			func(s0,s1,s2,s3,s4,s5,s6,s7,s8,s9);
		}
		catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
			string what = e.what();
			PE(what, "LUA");
		}
	}
}

void CompileScript(string Path) {
	try {
		lua.script_file(Path);
	}
	catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
		string what = e.what();
		PE(what, "LUA");
	}
}

void LuaCompile() {
	P("LUA", "Start Lua compile...");
	lua.open_libraries(sol::lib::package);

	EmptyImage = GetSessionInfo("SourcePath") + "engine/default.png";

	/*Загрузка скриптов из игры*/
	const string package_path_ = lua["package"]["path"];
	lua["package"]["path"] = package_path_ + (!package_path_.empty() ? ";" : "") + GetSessionInfo("SourcePath") + "?.lua";
	for (auto& p : std::filesystem::recursive_directory_iterator(GetSessionInfo("SourcePath"))) {
		if (p.is_directory()) {
			const string package_path = lua["package"]["path"];
			lua["package"]["path"] = package_path + (!package_path.empty() ? ";" : "") + p.path().string() + "/?.lua";
		}
	}
	SetSessionInfo("LuaLibs", lua["package"]["path"]);
	P("LUA", "Lua libraries are loaded!");

	/*Конструкторы*/
	lua.new_usertype<l_Color>("Color",
		sol::constructors<l_Color(), l_Color(int, int, int, int), l_Color(int, int, int), l_Color(int, int), l_Color(int)>(),
		"SetR", &l_Color::SetR,
		"SetG", &l_Color::SetG,
		"SetB", &l_Color::SetB,
		"SetA", &l_Color::SetA,
		"GetR", &l_Color::GetR,
		"GetG", &l_Color::GetG,
		"GetB", &l_Color::GetB,
		"GetA", &l_Color::GetA,
		"Invert", &l_Color::Invert,
		"InvertAlpha", &l_Color::InvertAlpha,
		"InvertAll", &l_Color::InvertAll,
		"Gray", &l_Color::Gray,
		"Contrast", &l_Color::Contrast
	);

	lua.new_usertype<l_Vector2>("Vector2",
		sol::constructors<l_Vector2(), l_Vector2(float), l_Vector2(float, float)>(),
		"x", &l_Vector2::x,
		"y", &l_Vector2::y
	);

	lua.new_usertype<l_Vector3>("Vector3",
		sol::constructors<l_Vector3(), l_Vector2(float), l_Vector2(float, float), l_Vector3(float, float, float)>(),
		"x", &l_Vector3::x,
		"y", &l_Vector3::y,
		"z", &l_Vector3::z
	);

	lua.new_usertype<l_Vector4>("Vector4",
		sol::constructors<l_Vector4(), l_Vector2(float), l_Vector2(float, float), l_Vector3(float, float, float), l_Vector4(float, float, float, float)>(),
		"x", &l_Vector4::x,
		"y", &l_Vector4::y,
		"z", &l_Vector4::z,
		"w", &l_Vector4::w
	);

	lua.new_usertype<l_Sound>("Sound",
		sol::constructors<l_Sound()>()
	);

	lua.new_usertype<l_Vertex>("Vertex",
		sol::constructors<l_Vertex(), l_Vertex(l_Vector2, l_Color), l_Vertex(l_Vector2)>(),
		"Color", &l_Vertex::Color,
		"Position", &l_Vertex::Position
	);

	/*Константы*/
	lua["Pi"] = sol::as_table(3.14159265358979323846);
	lua["Sqrt2"] = sol::as_table(1.41421356237309504880);
	lua["E"] = sol::as_table(2.71828182845904523536);
	lua["Log2E"] = sol::as_table(1.44269504088896340736);
	lua["Log10E"] = sol::as_table(0.434294481903251827651);
	lua["Ln2"] = sol::as_table(0.693147180559945309417);
	lua["Ln10"] = sol::as_table(2.30258509299404568402);
	lua["EarthGravity"] = sol::as_table(9.80665);
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
	lua["SafeMode"] = sol::as_table(StringToBool(GetSettingsInfo("SafeMode")));
	lua["ConsoleEnabled"] = sol::as_table(StringToBool(GetSettingsInfo("Console")));
	lua["DebugMode"] = sol::as_table(StringToBool(GetSessionInfo("Debug")));
	lua["Infinity"] = sol::as_table(std::numeric_limits<int>::max());
	lua["Red"] = sol::as_table(l_Color(255, 0, 0, 255));
	lua["Green"] = sol::as_table(l_Color(0, 255, 0, 255));
	lua["Blue"] = sol::as_table(l_Color(0, 0, 255, 255));
	lua["Yellow"] = sol::as_table(l_Color(255, 255, 0, 255));
	lua["White"] = sol::as_table(l_Color(255, 255, 255, 255));
	lua["Black"] = sol::as_table(l_Color(0, 0, 0, 255));
	lua["Gray"] = sol::as_table(l_Color(128, 128, 128, 255));
	lua["Orange"] = sol::as_table(l_Color(255, 128, 0, 255));
	lua["Purple"] = sol::as_table(l_Color(255, 0, 255, 255));
	lua["Cyan"] = sol::as_table(l_Color(0, 255, 255, 255));
	lua["Transparent"] = sol::as_table(l_Color(0, 0, 0, 0));
	lua["Up"] = sol::as_table(l_Vector2(0, 1));
	lua["Down"] = sol::as_table(l_Vector2(0, -1));
	lua["Right"] = sol::as_table(l_Vector2(1, 0));
	lua["Left"] = sol::as_table(l_Vector2(-1, 0));
	lua["Front"] = sol::as_table(l_Vector3(0, 0, 1));
	lua["Back"] = sol::as_table(l_Vector3(0, 0, -1));
	lua["ErrorColor"] = sol::as_table(ErrorColor);
	lua["StringMax"] = sol::as_table(4294967295);
	lua["EngineChars"] = sol::as_table(GetCharsCount());
	lua["Vector2Zero"] = sol::as_table(l_Vector2(0, 0));
	lua["Vector2One"] = sol::as_table(l_Vector2(1, 1));
	lua["Vector3Zero"] = sol::as_table(l_Vector3(0, 0, 0));
	lua["Vector3One"] = sol::as_table(l_Vector3(1, 1, 1));
	lua["Vector4Zero"] = sol::as_table(l_Vector4(0, 0, 0, 0));
	lua["Vector4One"] = sol::as_table(l_Vector4(1, 1, 1, 1));

	/*Функции*/
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
	lua.set_function("GetMainWindow", &l_MainWindow);
	lua.set_function("GetSeed", &l_Seed);
	lua.set_function("SetSeed", &l_SetSeed);
	lua.set_function("GetWindowSize", &l_GetWindowSize);
	lua.set_function("SetWindowSize", &l_SetWindowSize);
	lua.set_function("SetWindowTitle", &l_SetWindowTitle);
	lua.set_function("SetWindowAutoScale", &l_SetWindowAutoScale);
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
	lua.set_function("RRandom", &l_RRandom);
	lua.set_function("GetWindowPosition", &l_GetWindowPosition);
	lua.set_function("SetWindowPosition", &l_SetWindowPosition);
	lua.set_function("Frac", &l_Frac);
	lua.set_function("JSONTable", &l_JSONTable);
	lua.set_function("Pairs", &l_Pairs);
	lua.set_function("ToString", &l_ToString);
	lua.set_function("GetType", &l_GetType);
	lua.set_function("OpenLink", &l_OpenLink);
	lua.set_function("ActiveTime", &l_ActiveTime);
	lua.set_function("SetCameraPosition", &l_SetCameraPosition);
	lua.set_function("GetCameraPosition", &l_GetCameraPosition);
	lua.set_function("SetSpritePosition", &l_SetSpritePosition);
	lua.set_function("CreateScene", &l_CreateScene);
	lua.set_function("SetSceneBackgroundColor", &l_SetSceneBackgroundColor);
	lua.set_function("SetWindowScene", &l_SetSceneWindow);
	lua.set_function("CreateSprite", &l_CreateSprite);
	lua.set_function("GetFPS", &l_GetFPS);
	lua.set_function("CycleRender", &l_CycleRender);
	lua.set_function("SetCameraZoom", &l_SetCameraZoom);
	lua.set_function("GetCameraZoom", &l_GetCameraZoom);
	lua.set_function("GetMousePosition", &l_GetMousePosition);
	lua.set_function("GetMouseWPosition", &l_GetMouseWPosition);
	lua.set_function("ScreenToWorld", &l_ScreenToWorld);
	lua.set_function("WorldToScreen", &l_WorldToScreen);
	lua.set_function("SetWindowEventMousePress", &l_SetWindowEventMousePress);
	lua.set_function("SetWindowEventMouseRelease", &l_SetWindowEventMouseRelease);
	lua.set_function("SetWindowEventMouseHold", &l_SetWindowEventMouseHold);
	lua.set_function("Sign", &l_Sign);
	lua.set_function("SetDiscordActivityTitle", &l_SetDiscordActivityTitle);
	lua.set_function("SetDiscordActivityDescription", &l_SetDiscordActivityDescription);
	lua.set_function("GetUserName", &l_GetUserName);
	lua.set_function("CheckInternet", &l_CheckInternet);
	lua.set_function("ToBool", &l_ToBool);
	lua.set_function("RunLua", &l_RunLua);
	lua.set_function("Repeat", &l_Repeat);
	lua.set_function("ProgramLaunched", &l_ProgramLaunched);
	lua.set_function("DiscordLaunched", &l_DiscordLaunched);
	lua.set_function("ProgramsLaunched", &l_ProgramsLaunched);
	lua.set_function("StopProgram", &l_StopProgram);
	lua.set_function("ExitPC", &l_ExitPC);
	lua.set_function("PointOutsideWindow", &l_PointOutsideWindow);
	lua.set_function("GetFilesFromDirectory", &l_GetFilesFromDirectory);
	lua.set_function("GetFileName", &l_GetFileName);
	lua.set_function("GetFileType", &l_GetFileType);
	lua.set_function("PrintFast", &l_PrintFast);
	lua.set_function("SetSpriteSize", &l_SetSpriteSize);
	lua.set_function("SetSpriteCorner", &l_SetSpriteCorner);
	lua.set_function("SetFPSTarget", &l_SetFPSTarget);
	lua.set_function("SetEventPrint", &l_SetEventPrint);
	lua.set_function("SetWindowTransparency", &l_SetWindowTransparency);
	lua.set_function("GetWindowTransparency", &l_GetWindowTransparency);
	lua.set_function("OpenFile", &l_OpenFile);
	lua.set_function("HasFile", &l_HasDirectory);
	lua.set_function("OpenDirectory", &l_OpenFile);
	lua.set_function("SetSpriteColor", &l_SetSpriteColor);
	lua.set_function("GetSpriteColor", &l_GetSpriteColor);
	lua.set_function("SetSpriteLayer", &l_SetSpriteLayer);
	lua.set_function("GetSpriteLayer", &l_GetSpriteLayer);
	lua.set_function("FRandom", &l_FRandom);
	lua.set_function("FRRandom", &l_FRRandom);
	lua.set_function("GetSprites", &l_GetSprites);
	lua.set_function("NewThread", &l_NoWait);
	lua.set_function("GetLanguage", &l_GetLanguage);
	lua.set_function("GetSystemLanguage", &l_GetSystemLanguage);
	lua.set_function("HasSprite", &l_HasSprite);
	lua.set_function("SetSpriteTexture", &l_SetSpriteTexture);
	lua.set_function("SetSpriteSizeByTexture", &l_SetSpriteSizeByTexture);
	lua.set_function("GetTextureSize", &l_GetTextureSize);
	lua.set_function("GetSpriteSize", &l_GetSpriteSize);
	lua.set_function("GetSpritePosition", &l_GetSpritePosition);
	lua.set_function("SetSpriteMirrorX", &l_SetSpriteMirrorX);
	lua.set_function("SetSpriteMirrorY", &l_SetSpriteMirrorY);
	lua.set_function("GetSpriteMirrorX", &l_GetSpriteMirrorX);
	lua.set_function("GetSpriteMirrorY", &l_GetSpriteMirrorY);
	lua.set_function("SetSpriteUVCorner", &l_SetSpriteUVCorner);
	lua.set_function("SetSpriteRotation", &l_SetSpriteOrientation);
	lua.set_function("Lerp", &l_Lerp);
	lua.set_function("ToRad", &l_ToRad);
	lua.set_function("ToDeg", &l_ToDeg);
	lua.set_function("GetSpriteRotation", &l_GetSpriteRotation);
	lua.set_function("SetSpriteCenter", &l_SetSpriteCenter);
	lua.set_function("GetSpriteCenter", &l_GetSpriteCenter);
	lua.set_function("GetWindows", &l_GetWindows);
	lua.set_function("GetSceneWindow", &l_GetWindow);
	lua.set_function("GetWindowScene", &l_GetScene);
	lua.set_function("SetDesktopBackground", &l_SetDesktopBackground);
	lua.set_function("GetVolume", &l_GetVolume);
	lua.set_function("SetVolume", &l_SetVolume);
	lua.set_function("LoadTexture", &l_LoadTexture);
	lua.set_function("SetSpriteVisible", &l_SetSpriteVisible);
	lua.set_function("GetSpriteVisible", &l_GetSpriteVisible);
	lua.set_function("SetTextureBlur", &l_SetTextureBlur);
	lua.set_function("GetTextureColors", &l_GetTextureColors);
	lua.set_function("SetConsoleTitle", &l_SetConsoleTitle);
	lua.set_function("CreateTexture", &l_CreateTexture);
	lua.set_function("WriteImage", &l_WriteImage);
	lua.set_function("SetSpriteShader", &l_SetSpriteShader);
	lua.set_function("SetSpriteHeight", &l_SetSpriteHeight);
	lua.set_function("Pow", &l_Pow);
	lua.set_function("RemoveFile", &l_RemoveFile);
	lua.set_function("PlayBeep", &l_PlayBeep);
	lua.set_function("GetSpriteShader", &l_GetSpriteShader);
	lua.set_function("GetSpriteHeight", &l_GetSpriteHeight);
	lua.set_function("TestFunction", &l_TestFunction);
	lua.set_function("GetDiscordUserInfo", &l_GetDiscordUserInfo);
	lua.set_function("GetDiscordCurrentUser", &l_GetDiscordCurrentUser);
	lua.set_function("CreateText", &l_CreateText);
	lua.set_function("CreateFont", &l_CreateFont);
	lua.set_function("SetTextPosition", &l_SetTextPosition);
	lua.set_function("SetTextFont", &l_SetTextFont);
	lua.set_function("SetTextColor", &l_SetTextColor);
	lua.set_function("GetASCIIChar", &l_GetASCIIChar);
	lua.set_function("SetTextText", &l_SetTextText);
	lua.set_function("SetTextHeight", &l_SetTextHeight);
	lua.set_function("StringToColors", &l_StringToColors);
	lua.set_function("SetShowCursor", &l_ShowCursor);
	lua.set_function("Even", &l_Even);
	lua.set_function("SetTextMono", &l_SetTextMono);
	lua.set_function("GetShowCursor", &l_GetShowCursor);
	lua.set_function("GetTextMono", &l_GetTextMono);
	lua.set_function("GetTextText", &l_GetTextText);
	lua.set_function("GetTextHeight", &l_GetTextHeight);
	lua.set_function("GetTextPosition", &l_GetTextPosition);
	lua.set_function("GetTextFont", &l_GetTextFont);
	lua.set_function("GetTextColor", &l_GetTextColor);
	lua.set_function("GetSpriteUVCorner", &l_GetSpriteCorner);
	lua.set_function("GetSpriteCorner", &l_GetSpriteCorner);
	lua.set_function("GetUnicodeChar", &l_GetUnicodeChar);
	lua.set_function("GetEngineChar", &l_GetEngineChar);
	lua.set_function("MessageBox", &l_MessageBox);
	lua.set_function("SetWindowImGui", &l_SetWindowImGui);
	lua.set_function("CreateImGuiWindow", &l_CreateImGuiWindow);
	lua.set_function("CreateImGuiElement", &l_CreateImGuiElement);
	lua.set_function("SetImGuiElementText", &l_SetImGuiElementText);
	lua.set_function("SetImGuiElementColor", &l_SetImGuiElementColor);
	lua.set_function("RandomColor", &l_RandomColor);
	lua.set_function("SetImGuiElementEvent", &l_SetImGuiElementEvent);

	lua.set_function("SetImGuiElementPoint", &l_SetImGuiElementPoint);
	lua.set_function("SetImGuiElementStringValue", &l_SetImGuiElementStringValue);
	lua.set_function("SetImGuiElementConnect", &l_SetImGuiElementConnect);
	lua.set_function("SetImGuiElementTooltip", &l_SetImGuiElementTooltip);
	lua.set_function("SetImGuiFontScale", &l_SetImGuiFontSize);
	lua.set_function("SetImGuiWindowVisible", &l_SetImGuiWindowVisible);
	lua.set_function("SetImGuiElementValue", &l_SetImGuiElementValue);
	lua.set_function("GetImGuiElementValue", &l_GetImGuiElementValue);

	lua.set_function("GetWindowTitle", &l_WIP);
	lua.set_function("GetWindowAutoScale", &l_WIP);
	lua.set_function("GetWindowResizable", &l_WIP);
	lua.set_function("GetTextureBlur", &l_WIP);
	lua.set_function("GetSceneBackgroundColor", &l_WIP);
	lua.set_function("GetWindowImGui", &l_WIP);
	lua.set_function("GetImGuiElementText", &l_WIP);
	lua.set_function("GetImGuiElementColor", &l_WIP);
	lua.set_function("GetImGuiElementPoint", &l_WIP);
	lua.set_function("GetImGuiElementStringValue", &l_WIP);
	lua.set_function("GetImGuiElementConnect", &l_WIP);
	lua.set_function("GetImGuiElementTooltip", &l_WIP);
	lua.set_function("GetImGuiFontScale", &l_WIP);
	lua.set_function("GetImGuiWindowVisible", &l_WIP);

	lua.set_function("RemoveMagicalNumber", &l_RemoveMagicalNumber);

	P("LUA", "Lua functions and etc. are loaded!");
	P("LUA", "Start '"+ GetEngineInfo("StartScript") +".lua' script...");
	CompileScript(GetSessionInfo("SourcePath") + GetEngineInfo("StartScript") + ".lua");
}