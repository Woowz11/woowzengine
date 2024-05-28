#pragma warning(disable : 4244)
#pragma warning(disable : 4267)

#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <filesystem>
#include <regex>
#include <map>
#include <unordered_map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <glad/gl.h>
#include "OpenGame.h"
#include "GLFW.h"
#include "time.h"
#include "Base.h"
#include "Console.h"
#include "Easyer.h"
#include "Files.h"
#include "Logger.h"
#include "WindowsElements.h"

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE(x) STRINGIZE(x)

using namespace std;
using json = nlohmann::json;

string GamePath = "";
string SessionInfoPath = "woowzengine/temporary/sessioninfo";
string LogsStyle = "%b[%s:%m:%h][%t] %c";

int ErrorsCount = 0;

/*Получить знак числа, минус или плюс*/
int GetNumberZnak(float num) {
	int result = 1;
	if (num < 0) {
		result = -1;
	}
	return result;
}

/*Тоже самое что to_string() только без нулей в конце*/
string DoubleToString(double value) {
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(15) << value;
	std::string str = stream.str();
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	if (str.back() == '.') {
		str.pop_back();
	}
	return str;
}

/*Получить путь без файла*/
string GetPathWithoutFileName(string path) {
	return ReplaceString(path, FileName(path), "");
}

/*Получить название файла*/
string FileName(string path) {
	return path.substr(path.find_last_of("/\\") + 1);
}

/*Проверяем безопасный режим включен или нет*/
bool SafeMode() {
	return StringToBool(GetSettingsInfo("SafeMode"));
}

/*Выход из приложения*/
void Exit() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	HANDLE hProcess = GetCurrentProcess();
	TerminateProcess(hProcess, 0);
	ExitProcess(0);
	exit(EXIT_SUCCESS);
}

/*Получить дробную часть*/
float GetFractionalPart(int i) {
	return i - round(i);
}

/*Установить сид*/
void SetRandomSeed(int seed, bool dontsaveseed) {
	srand(seed);
	if (!dontsaveseed) { SetSessionInfo("Seed", to_string(seed)); }
}

/*Получить случайное число*/
float Random(float min,float max,bool dontsaveseed) {
	if (min > max) { PE("Minimum number cannot be > maximum number! Random(" + to_string(min) + "," + to_string(max) + ")", "E0004"); return -1; }
	else {
		return (Random(dontsaveseed) * (max - min)) + min;
	}
}
int fastseed = (int)std::time(nullptr);
float Random(bool dontsaveseed) {
	int seed_ = (dontsaveseed?fastseed:StringToInt(GetSessionInfo("Seed")));
	int seed = ((seed_ + 693206737) ^ (seed_ << 13) ^ (seed_ >> 17)) % 100000000 + (rand()/RAND_MAX)*100000000;
	SetRandomSeed(seed, dontsaveseed);
	if (dontsaveseed) {
		fastseed = seed;
	}
	return (float)static_cast<double>(rand()) / RAND_MAX;
}

/*Установка Base.cpp в проект*/
void BaseInstall(string GamePath_) {
	GamePath = GamePath_;
	std::setlocale(LC_NUMERIC, "POSIX");
	if (JSONValid(GamePath + SessionInfoPath)) {
		if (GetEngineInfoIE("LogStyle") != "WARN_EMPTY") {
			LogsStyle = GetEngineInfo("LogStyle");
		}
	}
	if (StringEmpty(LogsStyle)) { MessageBoxFatal("LogStyle (engine.json) can't be empty!", "C0014", true); }
}

#pragma region ConsoleZone
/*Отправка сообщения в консоль и остальное*/
void Print(string Text, int Color, bool OnlyLog) {
	if (!OnlyLog) {
		PrintToConsole(Text, Color);
	}
	PrintToLog(Text);
}
void Print(int Text, int Color, bool OnlyLog) {
	Print(to_string(Text),Color, OnlyLog);
}

/*Делает символы в строке большими*/
string Uppercase(string Str) {
	transform(Str.begin(), Str.end(), Str.begin(),
		[](unsigned char c) { return toupper(c); });
	return Str;
}

/*Делает символы в строке маленькими*/
string Lowercase(string Str) {
	transform(Str.begin(), Str.end(), Str.begin(),
		[](unsigned char c) { return tolower(c); });
	return Str;
}

/*Первращает строку в сообщение для логов*/
string ConvertTextToConsoleLogMessage(string Text, string Module, string c) {
	return ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(LogsStyle, "%ms", GetTime("ms")), "%tt", Uppercase(Module)), "%c", Text), "%t", Uppercase(FillString(Module, 7, ' '))), "%b", c), "%w", GetTime("w")), "%y", GetTime("y")), "%mn", GetTime("mn")), "%d", GetTime("d")), "%h", GetTime("h")), "%m", GetTime("m")), "%s", GetTime("s"));
}
/*Отправить сообщение*/
void P(string Module, string Text, int color, string ch) {
	bool next = RunPrintFunction(Text, "Unknown", Module, "", color, ch);
	Print(ConvertTextToConsoleLogMessage(Text, Module, ch), color, !next);
}
/*Отправить обычное сообщение*/
void PP(string Text) {
	bool next = RunPrintFunction(Text, "Print", "PRINT", "", 7, "*");
	Print(ConvertTextToConsoleLogMessage(Text, "PRINT", "*"), 7, !next);
}
/*Отправить ошибку*/
void PE(string Text,string ErrorCode) {
	bool next = RunPrintFunction(Text, "Error", "ERROR", ErrorCode, 12, "!");
	ErrorsCount++;
	Print(ConvertTextToConsoleLogMessage(Text + " - " + ErrorCode, "ERROR", "!"), 12, !next);
	if (ErrorsCount >= 10) {
		LogsErrors();
	}
}
/*Отправить предупреждение*/
void PW(string Text,string Code) {
	bool next = RunPrintFunction(Text, "Warning", "WARN", Code, 14, "?");
	Print(ConvertTextToConsoleLogMessage(Text + " - " + Code, "WARN", "?"), 14, !next);
}
/*Отправить фатальую ошибку*/
void PF(string Text,string Code, bool DontPrint) {
	RunPrintFunction(Text, "Fatal", "FATAL", Code, 4, "*");
	if (!DontPrint) { Print(ConvertTextToConsoleLogMessage(Text+" - "+Code, "FATAL", "#"), 4); }
	MessageBoxFatal(Text,Code,DontPrint);
	Exit();
}
#pragma endregion ConsoleZone

/*Создаёт папку если её нету*/
bool GetOrCreateFolder(string Dir) {
	if (HasDirectory(Dir)) {
		return true;
	}
	CreateDirectory(Dir);
	return false;
}

/*Создаёт файл если его нету*/
bool GetOrCreateFile(string Dir) {
	if (HasDirectory(Dir)) {
		return true;
	}
	CreateFile(Dir);
	return false;
}

/*Остановить процесс на определённое кол-во милисекунд (Milliseconds > 0)*/
void Wait(int Milliseconds) {
	if (Milliseconds <= 0) { PE("Wait(" + to_string(Milliseconds) + ") function only accepts a number >0!", "E0000"); }
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
	}
}

/*Уберает пробелы с конца и начал строки*/
string Trim(string s) {
	int i = 0, j;
	while ((s[i] == ' ') || (s[i] == '\t'))
	{
		i++;
	}
	if (i > 0)
	{
		for (j = 0; j < s.length(); j++)
		{
			s[j] = s[j + i];
		}
		s[j] = '\0';
	}

	i = s.length() - 1;
	while ((s[i] == ' ') || (s[i] == '\t'))
	{
		i--;
	}
	if (i < (s.length() - 1))
	{
		s[i + 1] = '\0';
	}
	return s;
}

/*Есть кириллица или нет*/
bool OnlyASCII(const std::string& str) {
	for (char c : str) {
		if (static_cast<unsigned char>(c) > 127) {
			return false;
		}
	}
	return true;
}

/*Может называться в windows*/
bool NameWindowsAccept(string Str, bool ThatEnd, bool ThatPath) {
	bool result = FindChar(Str, '*') || FindChar(Str, '?') || FindChar(Str, '"') || FindChar(Str, '<') || FindChar(Str, '>') || FindChar(Str, '|') || FindChar(Str, '+');
	if (ThatEnd) {
		result = result || FindChar(Str, ' ');
	}
	if (!ThatPath) {
		result = result || FindChar(Str, '\\') || FindChar(Str, '/') || FindChar(Str, ':') || FindChar(Str, '.');
	}
	return !result;
}

/*Ищет симбол в строке*/
bool FindChar(string Str, char chr) {
	return Str.find(chr) != string::npos;
}

/*Проверяет пустая строка или нет*/
bool StringEmpty(string Str) {
	if (Str.empty()) { return true; }
	string Str_ = Trim(Str);
	return Str_.empty();
}

/*Получить название файла из пути*/
string GetFileName(string Path) {
	return Path.substr(Path.find_last_of("/\\") + 1);
}

/*Получить информацию из файла game.json*/
string GetGameInfo(string ID) {
	string p = GetSessionInfo("GameJson");
	if (!HasDirectory(p)) { PF("game.json not found!", "C0007", true); return "ERROR_C0007"; }
	if (!JSONValid(p)) { PF("game.json corrupted!\nTry deleting the file!", "C0008", true); return "ERROR_C0008"; }
	return ReadJson(p, ID);
}
string GetGameInfoIE(string ID) {
	if (!HasDirectory(GamePath + SessionInfoPath)) { return "WARN_EMPTY"; }
	string p = GetSessionInfoIE("GameJson");
	if (!HasDirectory(p)) { return "WARN_EMPTY"; }
	if (!JSONValid(p)) { PF("game.json corrupted!\nTry deleting the file!", "C0008", true); return "ERROR_C0008"; }
	return ReadJson(p, ID, "WARN_EMPTY");
}

/*Получить информацию из файла engine.json*/
string GetEngineInfo(string ID) {
	string p = GetSessionInfo("EngineJson");
	if (p == "WARN_EMPTY") { return p; }
	if (!HasDirectory(p)) { PF("engine.json not found!", "C0009", true); return "ERROR_C0009"; }
	if (!JSONValid(p)) { PF("engine.json corrupted!\nTry deleting the file!", "C0010", true); return "ERROR_C0010"; }
	return ReadJson(p, ID);
}
string GetEngineInfoIE(string ID) {
	if (!HasDirectory(GamePath + SessionInfoPath)) { return "WARN_EMPTY"; }
	string p = GetSessionInfoIE("EngineJson");
	if (p == "WARN_EMPTY") { return p; }
	if (!HasDirectory(p)) { return "WARN_EMPTY"; }
	if (!JSONValid(p)) { PF("engine.json corrupted!\nTry deleting the file!", "C0010", true); return "ERROR_C0010"; }
	return ReadJson(p, ID, "WARN_EMPTY");
}

/*Получить информацию из файла settings.json*/
string GetSettingsInfo(string ID) {
	string p = GetSessionInfo("SettingsJson");
	if (p == "WARN_EMPTY") { return p; }
	if (!HasDirectory(p)) { PF("settings.json not found!", "C0009", true); return "ERROR_C0029"; }
	if (!JSONValid(p)) { PF("settings.json corrupted!\nTry deleting the file!", "C0010", true); return "ERROR_C0030"; }
	return ReadJson(p, ID);
}

/*Получить информацию из файла sessioninfo*/
string GetSessionInfo(string ID) {
	if (!HasDirectory(GamePath + SessionInfoPath)) { PF("Sessioninfo not found!", "C0001", true); return "ERROR_C0001"; }
	if (!JSONValid(GamePath + SessionInfoPath)) { PF("Sessioninfo corrupted!", "C0002", true); return "ERROR_C0002"; }
	return ReadJson(GamePath + SessionInfoPath, ID);
}
string GetSessionInfoIE(string ID) {
	if (!HasDirectory(GamePath + SessionInfoPath)) { return "WARN_EMPTY"; }
	if (!JSONValid(GamePath + SessionInfoPath)) { PF("Sessioninfo corrupted!", "C0002", true); return "ERROR_C0002"; }
	return ReadJson(GamePath + SessionInfoPath, ID, "WARN_EMPTY");
}

/*Заменить информацию в файле sessioninfo*/
void SetSessionInfo(string ID, string Value) {
	if (!HasDirectory(GamePath + SessionInfoPath)) { PF("Sessioninfo not found!", "C0016", true); }
	WriteToJson(GamePath + SessionInfoPath, ID, Value);
}

/*Создать переменную в JSON если она не существует*/
void CreateValueJson(string Path, string ID, string DefaultValue) {
	if (ReadJson(Path, ID, "notfound") == "notfound") {
		WriteToJson(Path, ID, DefaultValue);
	}
}

/*Записать переменную в JSON*/
void WriteToJson(string Path, string ID, string Value) {
	json data = json::parse(ReadFile(Path));
	data[ID] = Value;
	ofstream output_file(Path);
	output_file << data.dump(4);
	output_file.close();
}

/*Проверить путь на запрещёные символы*/
bool CheckPathToSymbols(string path) {
	if (NameWindowsAccept(path,false,true)) {
		return true;
	}
	else {
		PE("Path ("+path+") contains illegal characters in windows (* ? \" < > | +)","E0010"); return false;
	}
}

/*Прочитать переменную в JSON*/
string ReadJson(string Path, string ID, string IfNotFound) {
	if (JSONValid(Path)) {
		string JSON = ReadFile(Path);
		json data = json::parse(JSON);
		if (data.contains(ID)) {
			return data[ID];
		}
		else {
			if (IfNotFound == "") {
				PE("There is no such variable [" + ID + "] in the JSON file [" + Path + "]!", "E0001");
				return "ERROR_E0001";
			}
			else {
				return IfNotFound;
			}
		}
	}
	else {
		PE("JSON file [" + Path + "] has an incorrect structure! ReadJson('"+ID+"','"+IfNotFound+"')", "E0002");
		return "ERROR_E0002";
	}
}

/*Получить переменные из JSON*/
map<string,string> ReadAllJson(string Path) {
	if (JSONValid(Path)) {
		string JSON = ReadFile(Path);
		json data = json::parse(JSON);
		map<string, string> result = {};
		for (json::iterator it = data.begin(); it != data.end(); ++it) {
			result[it.key()] = it.value().get<std::string>();
		}
		return result;
	}
	else {
		PE("JSON file [" + Path + "] has an incorrect structure! ReadAllJson()", "E0015");
		return map<string,string>();
	}
}

/*Заполняет пустоты символом*/
string FillString(string Str, int Width, char Symbol, bool Invert) {
	if (Width <= 0) { return Str; }
	string Result = Str;
	int AddThat = Width - Str.length();
	if (AddThat <= 0) { return Str; }
	for (int i = 0; i < AddThat; i++) {
		if (Invert) {
			Result = Symbol + Result;
		}
		else {
			Result = Result + Symbol;
		}
	}
	return Result;
}

/*Получить часть времени используя теги*/
/*
   "s"  - Возвращает секунды
   "m"  - Возвращает минуты
   "h"  - Возвращает часы
   "d"  - Возвращает дни
   "mn" - Возвращает месяца
   "y"  - Возвращает года
   "w"  - Возвращает недели
   "ms" - Возвращает миллисекунды
*/
string GetTimeComponent(std::chrono::system_clock::time_point Time, string Tag) {
	std::time_t time = std::chrono::system_clock::to_time_t(Time);
	std::tm Now;
	localtime_s(&Now, &time);
	int Val = -1;
	int AddZeros = 0;

	if (Tag == "s") {
		Val = Now.tm_sec;
		AddZeros = 2;
	}
	else if (Tag == "m") {
		Val = Now.tm_min;
		AddZeros = 2;
	}
	else if (Tag == "h") {
		Val = Now.tm_hour;
		AddZeros = 2;
	}
	else if (Tag == "d") {
		Val = Now.tm_mday;
		AddZeros = 2;
	}
	else if (Tag == "mn") {
		Val = Now.tm_mon + 1;
		AddZeros = 2;
	}
	else if (Tag == "y") {
		Val = Now.tm_year + 1900;
		AddZeros = 4;
	}
	else if (Tag == "w") {
		Val = Now.tm_wday;
		AddZeros = 1;
	}
	else if (Tag == "ms") {
		auto duration = Time.time_since_epoch();
		Val = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000);
		AddZeros = 3;
	}
	if (Val == -1) { PE("Tag ["+Tag+"] does not exist in the function GetTimeComponent()!","E0003"); return "ERROR_E0003"; }
	return FillString(to_string(Val), AddZeros, '0', true);
}

/*Превращает строку в bool*/
bool StringToBool(string Str) {
	string Bool = Lowercase(Str);
	bool result = false;
	if (Bool == "true" || Bool == "yes" || Bool == "1" || Bool == "y" || Bool == "t" || Bool == "+") {
		result = true;
	}
	return result;
}

/*Получить текущее время используя теги*/
string GetTime(string Tag) {
	return GetTimeComponent(std::chrono::system_clock::now(),Tag);
}

/*Получить Value из Map используя Key*/
template <typename K, typename V>
V GetFromMapExtra(const map<K, V>& m, const K& key) {
	auto it = m.find(key);
	if (it != m.end()) {
		return it->second;
	}
	else {
		PE("Element not found! GetFromMap('" + string(typeid(K).name()) + "','"+ string(typeid(V).name()) +"')", "E0007");
		return V{};
	}
	return V{};
}
template <typename K, typename V>
V GetFromMapExtra(const unordered_map<K, V>& m, const K& key) {
	auto it = m.find(key);
	if (it != m.end()) {
		return it->second;
	}
	else {
		PE("Element not found! GetFromMap('" + string(typeid(K).name()) + "','" + string(typeid(V).name()) + "')", "E0007");
		return V{};
	}
	return V{};
}
GLuint GetFromMap(unordered_map<string, GLuint> map, string id) {
	return GetFromMapExtra(map, id);
}
int GetFromMap(map<string, int> map, string id) {
	return GetFromMapExtra(map, id);
}
map<string,int> GetFromMap(map<string, map<string, int>> map, string id) {
	return GetFromMapExtra(map, id);
}

/*Получить элемент из list по номеру*/
template <typename T>
T GetFromListExtra(const std::list<T>& myList, int index) {
	if (index < 0 || index >= myList.size()) {
		PE("Index ["+to_string(index) + "] goes beyond the list! Max ["+to_string(myList.size()) + "]", "E0011");
		return T();
	}

	auto it = myList.begin();
	std::advance(it, index);

	return *it;
}
Vertex GetFromList(std::list<Vertex> list, int i) {
	return GetFromListExtra(list, i);
}

/*Конвертирует список в JSON формат*/
string ConvertToJSON(map<string, string> KeysValues) {
	string result = "{";
	for (auto Key_ = KeysValues.begin(); Key_ != KeysValues.end(); ++Key_) {
		string Key = Key_->first;
		string Val = GetFromMapExtra(KeysValues, Key);
		result = result + "\"" + Key + "\": \""+Val+"\"";
		if (next(Key_) != KeysValues.end()) {
			result = result + ",";
		}
	}
	result = result + "}";
	json data = json::parse(result);
	return data.dump(4);
}

/*Проверяет содержит ли JSON ошибки*/
bool JSONValid(string Path) {
	if (!HasDirectory(Path)) { return false; }
	return json::accept(ReadFile(Path));
}

/*Получить данные из файла*/
string ReadFile(string Path) {
	return GetFileInfo(Path);
}

/*Вписать данные в файл*/
void WriteToFile(string Path, string Text, bool AddToNextLine) {
	if (AddToNextLine) {
		AddFileInfo(Path,Text);
	}
	else {
		SetFileInfo(Path,Text);
	}
}

/*Заменяет символы в строке*/
string ReplaceString(string Str, string That = " ", string ToThat = "") {
	string result = Str;
	size_t pos = Str.find(That);
	while (pos != string::npos) {
		result.replace(pos, That.size(), ToThat);
		pos = result.find(That, pos + ToThat.size());
	}
	return result;
}

/*Говорит есть в строке строка*/
bool StringHasString(string Str, string WhatNeedFound) {
	if (Str.find(WhatNeedFound) != std::string::npos) {
		return true;
	}
	return false;
}