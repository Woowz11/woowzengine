#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include <filesystem>
#include <chrono>
#include <thread>
#include "LuaCompile.h"
#include "Base.h"
#include "Easyer.h"
#include "Files.h"

using namespace std;

/*Зона игры*/

/*Проверяет работает ли lua или нет*/
void l_CheckLua() {
	P("LUACHK", "Lua supported!");
}

/*Закрывает приложение*/
void l_Exit() {
	Exit();
}

/*Закрывает приложение*/
void l_Wait(int milisec) {
	Wait(milisec);
}

/*Вернуть случайное число от 0 до 1*/
float l_Random(float min,float max) {
	return Random(min,max);
}

/*Дробное число в целое*/
int l_Round(float val) {
	return FloatToInt(val);
}

/*Есть папка или файл в этой позиции*/
bool l_HasDirectory(string path) {
	return HasDirectory(path);
}

/*Получить данные из файла*/
string l_ReadFile(string path) {
	return ReadFile(path);
}

/*Записать данные в файл*/
void l_WriteFile(string path, string value) {
	if (SafeMode()) { PW("Function [WriteFile('"+path+"','"+value+"')] cannot be started in SafeMode!","LW0001"); }
	else {
		WriteToFile(path, value);
	}
}

/*Отправляет кастомное сообщение в консоль*/
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

/*Отправляет сообщение в консоль*/
void l_Print(string text) {
	PP(text);
}

/*Отправляет предупреждение в консоль*/
void l_Warn(string text,int code) {
	
	PW(text,"GW"+FillString(to_string(code),4,'0',true));
}

/*Отправляет ошибку в консоль*/
void l_Error(string text,int code) {
	PE(text, "GE" + FillString(to_string(code), 4, '0', true));
}

/*Отправляет фатальную ошибку в консоль*/
void l_Fatal(string text, int code) {
	PF(text, "GF" + FillString(to_string(code), 4, '0', true));
}

/*Отправляет сообщение в консоль (без формата лога)*/
void l_PrintClear(string text, int color) {
	if (color < 0 || color>255) { PE("The color must match 0<color<255 in PrintClear('"+text+"')!","L0002"); }
	else {
		int color_ = color;
		if (color == 0) { color_ = 8; }
		if (color == 8) { color_ = 0; }
		Print(text, color_);
	}
}

/*Превращает функцию в цикл*/
void l_Cycle(sol::function func, int milisec) {
	if (func == sol::nil) { PE("Function not found or does not exist for Cycle("+to_string(milisec)+")!","L0000"); }
	else {
		if (milisec <= 0) { PE("The time in Cycle(" + to_string(milisec) + ") cannot be <= 0!","L0001"); }
		else {

			try {
				auto cycle = [func, milisec]() {
						while (true) {
							std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
							func();
						}
					};
				std::thread thread(cycle);
				thread.detach();
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "LUA");
			}
		}
	}
}

/*Создать папки*/
void l_CreateDirectory(string path) {
	if (SafeMode()) { PW("Function [CreateDirectory('" + path + "')] cannot be started in SafeMode!", "LW0002"); }
	else {
		GetOrCreateFolder(path);
	}
}

/*Создать файл*/
void l_CreateFile(string pathandname, string source) {
		if (SafeMode()) { PW("Function [CreateFile('" + pathandname + "','"+source+"')] cannot be started in SafeMode!", "LW0003"); }
		else {
			GetOrCreateFile(pathandname);
			if (!StringEmpty(source)) {
				WriteToFile(pathandname, source);
			}
		}
}

/*Запись переменных в JSON*/
void l_WriteJSON(string path, string id, string val) {
	if (SafeMode()) { PW("Function [WriteJSON('" + path + "','" + id + "','"+val+"')] cannot be started in SafeMode!", "LW0004"); }
	else {
		WriteToJson(path, id, val);
	}
}

/*Получить данные из json файла*/
string l_ReadJSON(string path, string id) {
	if (HasDirectory(path)) { PE("Json file [" + path + "] not found!", "L0006"); return "ERROR"; }
	if(!JSONValid(path)){ PE("Json file ["+path+"] corrupted!","L0005"); return "ERROR"; }
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
	return sqrt(f);
}

/*Логарифм*/
float l_Log(float f, float base) {
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

/*Тангец*/
float l_Tan(float f) {
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
float l_Clamp(float min_, float f, float max_) {
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

/*Арктангец*/
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
	return fmod(f, f2);
}

/*Остаток от деления но наоборот*/
float l_Rem(float f, float f2) {
	return remainder(f, f2);
}

/*Возводит в степень*/
float l_Pow(float f, float f2) {
	return pow(f,f2);
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
float l_Trunc(float f) {
	return trunc(f);
}

/*Разбивает значение с плавающей запятой на дробную и целую части*/
float l_Modf(float f, float * f2) {
	return modff(f, f2);
}

/*Зона woowzengine*/

sol::state lua{};
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

	/*Загрузка скриптов из игры*/
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

	/*Константы*/
	lua["Pi"] = sol::as_table(3.14159265358979323846);
	lua["Sqrt2"] = sol::as_table(1.41421356237309504880);
	lua["E"] = sol::as_table(2.71828182845904523536);
	lua["Log2E"] = sol::as_table(1.44269504088896340736);
	lua["Log10E"] = sol::as_table(0.434294481903251827651);
	lua["Ln2"] = sol::as_table(0.693147180559945309417);
	lua["Ln10"] = sol::as_table(2.30258509299404568402);
	string gamepath = GetSessionInfo("SourcePath");
	lua["GamePath"] = sol::as_table(gamepath.substr(0, gamepath.size() - 1));

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

	P("LUA", "Lua functions and etc. are loaded!");
	P("LUA", "Start start.lua script...");
	CompileScript(GetSessionInfo("SourcePath") + "start.lua");
}