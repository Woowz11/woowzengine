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
#include "Discord.h"

#include "Color.h"
#include "Vector2.h"
#include "Vertex.h"
#include "RenderElement.h"

#include "l_Color.h"
#include "l_Vector2.h"
#include "l_Vector3.h"
#include "l_Vector4.h"
#include "l_Texture.h"
#include "l_Sound.h"
#include "l_Vertex.h"
#include "l_Sprite.h"

using namespace std;

sol::state lua{};

/*Зона игры*/

string EmptyWindow = "New Window";
string EmptyScene = "New Scene";
string EmptySprite = "New Sprite";

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
			if (color < 0 || color>255) { PE("The color must match 0<color<255 in PrintCustom('" + Uppercase(ToString(module)) + "','" + ToString(text) + "')!", "L0004"); }
			int color_ = color;
			if (color == 0) { color_ = 8; }
			if (color == 8) { color_ = 0; }
			else {
				char chr = ' ';
				if (firstsymbol!=sol::nil) {
					if (!StringEmpty(ToString(firstsymbol))) {
						if (ToString(firstsymbol).size() > 1) {
							PW("Length of the first character '" + ToString(firstsymbol) + "' is >1 in PrintCustom('" + Uppercase(ToString(module)) + "','" + ToString(text) + "')!", "LW0000");
						}
						chr = ToString(firstsymbol).at(0);
					}
				}
				Print(ConvertTextToConsoleLogMessage(ToString(text), ToString(module), chr), color_);
			}
		}
}

/*Отправляет сообщение в консоль*/
void l_Print(sol::object text) {
	PP(ToString(text));
}

/*Отправляет предупреждение в консоль*/
void l_Warn(sol::object text,int code) {
	
	PW(ToString(text),"GW"+FillString(to_string(code),4,'0',true));
}

/*Отправляет ошибку в консоль*/
void l_Error(sol::object text,int code) {
	PE(ToString(text), "GE" + FillString(to_string(code), 4, '0', true));
}

/*Отправляет фатальную ошибку в консоль*/
void l_Fatal(sol::object text, int code) {
	PF(ToString(text), "GF" + FillString(to_string(code), 4, '0', true));
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
int l_Trunc(float f) {
	return round(f-GetFractionalPart(f));
}

/*Разбивает значение с плавающей запятой на дробную и целую части*/
float l_Modf(float f, float * f2) {
	return modff(f, f2);
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
	for (char& c : Str) {
		tbl.add(CharToString(c));
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
		return obj.as<std::string>().length();
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
	return Str.erase(pos, size);
}

/*Превращает строку в число*/
float l_ToNumber(sol::object Str_, sol::object IfError) {
	string Str = ToString(Str_, "");
	if (Str == "") { PE("String cannot be empty! ToNumber('" + Str + "')", "L0016"); return 0; }
	if (GetObjectType(IfError) != "number") {
		return StringToFloat(Str);
	}
	else {
		return StringToFloat(Str,ToNumber(IfError));
	}
}

/*Запуск команды cmd*/
void l_Cmd(sol::object command_) {
	string command = ToString(command_);
	if (SafeMode()) { PW("Function [Cmd('" + command + "')] cannot be started in SafeMode!", "LW0014"); }
	else {
		int result = system(StringToConstChar(command));
		if (result != 0) {
			PE("Cmd command failed! Cmd('"+command+"')","L0022");
		}
	}
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
	SetWindowToMain(ToString(id,EmptyWindow));
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

/*Получить размер окна X*/
int l_GetWindowX(sol::object id) {
	return GetWindowSize(ToString(id,EmptyWindow), false);
}

/*Получить размер окна Y*/
int l_GetWindowY(sol::object id) {
	return GetWindowSize(ToString(id, EmptyWindow), true);
}

/*Изменить размер экрана по X*/
void l_SetWindowX(sol::object id, int i) {
	SetWindowSize(ToString(id, EmptyWindow), false, i);
}

/*Изменить размер экрана по Y*/
void l_SetWindowY(sol::object id, int i) {
	SetWindowSize(ToString(id, EmptyWindow), true, i);
}

/*Изменяет название окна*/
void l_SetWindowTitle(sol::object id, sol::object title) {
	SetWindowTitle(ToString(id, EmptyWindow), ToString(title,"New Title"));
}

/*Изменение авторазмера у окна*/
void l_SetWindowAutoScale(sol::object id, sol::optional<bool> b) {
	if (b) {
		SetWindowAutosize(ToString(id, EmptyWindow), b.value());
	}
	else {
		SetWindowAutosize(ToString(id, EmptyWindow), true);
	}
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
void l_SetWindowResizable(sol::object id, bool b) {
	SetWindowResizable(ToString(id, EmptyWindow), b);
}

/*Вызывает функцию когда окно закрывается*/
void l_SetWindowEventClosed(sol::object id_, sol::function func) {
	string id = ToString(id_, EmptyWindow);
	if (func == sol::nil || !func.valid()) { PE("Function not found or does not exist for SetWindowEventClosed(" + id + ")!", "L0013"); }
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

/*Установить позицию окна по X*/
void l_SetWindowXPosition(sol::object id, int i) {
	SetWindowPosition(ToString(id, EmptyWindow), i, true);
}

/*Установить позицию окна по Y*/
void l_SetWindowYPosition(sol::object id, int i) {
	SetWindowPosition(ToString(id, EmptyWindow), i, false);
}

/*Получить позицию окна по X*/
int l_GetWindowXPosition(sol::object id) {
	return GetWindowPosition(ToString(id, EmptyWindow), true);
}

/*Получить позицию окна по Y*/
int l_GetWindowYPosition(sol::object id) {
	return GetWindowPosition(ToString(id, EmptyWindow), false);
}

/*Получает дробное число из числа*/
float l_Frac(float f) {
	return GetFractionalPart(f);
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
	for (auto& v : table) {
		StartFunction(func, {v.first,v.second});
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
		system(StringToConstChar("start \"\" " + html));
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
	CreateScene(ToString(id));
}

/*Установить цвет заднего фона у сцены*/
void l_SetSceneBackgroundColor(sol::object id, sol::object color) {
	SetSceneBackgroundColor(ToString(id), ObjToColor(color).ToCPP());
}

/*Установить сцену на окно*/
void l_SetSceneWindow(sol::object window, sol::object id) {
	SetWindowScene(ToString(window), ToString(id));
}

/*Создать спрайт*/
void l_CreateSprite(sol::object id, sol::object sceneid) {
	CreateSprite(ToString(id, EmptySprite), ToString(sceneid));
}

/*Установить позицию спрайта*/
void l_SetSpritePosition(sol::object sceneid, sol::object id, l_Vector2 position) {
	SetSpritePosition(ToString(sceneid), ToString(id, EmptySprite), position);
}

/*Установить цикл на рендер*/
void l_CycleRender(sol::function f) {
	SetDTFunction(f);
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

/*Зона woowzengine*/

l_Color ObjToColor(sol::object obj) {
	string type = GetObjectType(obj);
	if (type == "color") {
		return obj.as<sol::userdata>().as<l_Color>();
	}
	else if (type == "nil") {
		return l_Color(0, 0, 0, 255);
	}
	else {
		PE("Failed to convert the object [" + ToString(obj) + "] to color!", "L0015");
		return l_Color(255,0,255,255);
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

	/*Конструкторы*/
	lua.new_usertype<l_Color>("Color",
		sol::constructors<l_Color(), l_Color(int, int, int, int), l_Color(int, int, int), l_Color(int, int), l_Color(int)>(),
		"r", &l_Color::r,
		"g", &l_Color::g,
		"b", &l_Color::b,
		"a", &l_Color::a
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

	lua.new_usertype<l_Texture>("Texture",
		sol::constructors<l_Texture()>()
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
	lua.set_function("GetWindowSizeX", &l_GetWindowX);
	lua.set_function("GetWindowSizeY", &l_GetWindowY);
	lua.set_function("SetWindowSizeX", &l_SetWindowX);
	lua.set_function("SetWindowSizeY", &l_SetWindowY);
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
	lua.set_function("GetWindowX", &l_GetWindowXPosition);
	lua.set_function("GetWindowY", &l_GetWindowYPosition);
	lua.set_function("SetWindowX", &l_SetWindowXPosition);
	lua.set_function("SetWindowY", &l_SetWindowYPosition);
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

	P("LUA", "Lua functions and etc. are loaded!");
	P("LUA", "Start start.lua script...");
	CompileScript(GetSessionInfo("SourcePath") + "start.lua");
}