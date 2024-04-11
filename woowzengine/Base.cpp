#include <iostream>
#include <filesystem>
#include <regex>
#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Windows.h"
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

/*����� �� ����������*/
void Exit() {
	system("taskkill /F /T /IM woowzengine.exe");
}

/*��������� Base.cpp � ������*/
void BaseInstall(string GamePath_) {
	GamePath = GamePath_;
}

#pragma region ConsoleZone
/*�������� ��������� � ������� � ���������*/
void Print(string Text, int Color) {
	PrintToConsole(Text, Color);
	PrintToLog(Text);
}
void Print(int Text) {
	Print(to_string(Text));
}

/*������ ������� � ������ ��������*/
string Uppercase(string Str) {
	transform(Str.begin(), Str.end(), Str.begin(),
		[](unsigned char c) { return toupper(c); });
	return Str;
}

/*������ ������� � ������ ����������*/
string Lowercase(string Str) {
	transform(Str.begin(), Str.end(), Str.begin(),
		[](unsigned char c) { return tolower(c); });
	return Str;
}

/*���������� ������ � ��������� ��� �����*/
string ConvertTextToConsoleLogMessage(string Text, string Module, char StartSymbol) {
	string c(1, StartSymbol);
	return c + "[" + GetTime("s") + ":" + GetTime("m") + ":" + GetTime("h") + "][" + Uppercase(FillString(Module, 7, ' ')) + "] " + Text;
}
/*��������� ���������*/
void P(string Module, string Text) {
	Print(ConvertTextToConsoleLogMessage(Text, Module));
}
/*��������� ������� ���������*/
void PP(string Text) {
	Print(ConvertTextToConsoleLogMessage(Text,"PRINT",'*'),7);
}
/*��������� ������*/
void PE(string Text,string ErrorCode) {
	Print(ConvertTextToConsoleLogMessage(Text+" - "+ErrorCode, "ERROR", '!'), 12);
}
/*��������� ��������������*/
void PW(string Text,string Code) {
	Print(ConvertTextToConsoleLogMessage(Text+" - "+Code, "WARN", '?'), 14);
}
/*��������� �������� ������*/
void PF(string Text,string Code, bool DontPrint) {
	if (!DontPrint) { Print(ConvertTextToConsoleLogMessage(Text, "FATAL", '#'), 4); }
	MessageBoxFatal(Text,Code,DontPrint);
	Exit();
}
#pragma endregion ConsoleZone

/*������ ����� ���� � ����*/
bool GetOrCreateFolder(string Dir) {
	if (HasDirectory(Dir)) {
		return true;
	}
	CreateDirectory(Dir);
	return false;
}

/*������ ���� ���� ��� ����*/
bool GetOrCreateFile(string Dir) {
	if (HasDirectory(Dir)) {
		return true;
	}
	CreateFile(Dir);
	return false;
}

/*���������� ������� �� ����������� ���-�� ������ (Seconds > 0)*/
void Wait(float Seconds) {
	if (Seconds <= 0) { PE("The Wait() function only accepts a number greater than 0! Got ["+to_string(Seconds)+"]","E0000"); }
	else {
		Sleep(IntToDWORD(FloatToInt(Seconds * 1000)));
	}
}

/*������� ������� � ����� � ����� ������*/
string Trim(string s) {
	int i = 0, j;
	while ((s[i] == ' ') || (s[i] == '\t'))
	{
		i++;
	}
	if (i > 0)
	{
		for (j = 0; j < s.size(); j++)
		{
			s[j] = s[j + i];
		}
		s[j] = '\0';
	}

	i = s.size() - 1;
	while ((s[i] == ' ') || (s[i] == '\t'))
	{
		i--;
	}
	if (i < (s.size() - 1))
	{
		s[i + 1] = '\0';
	}
	return s;
}

/*����� ���������� � windows*/
bool NameWindowsAccept(string Str, bool ThatEnd) {
	bool result = FindChar(Str, '\\') || FindChar(Str, '/') || FindChar(Str, ':') || FindChar(Str, '*') || FindChar(Str, '?') || FindChar(Str, '"') || FindChar(Str, '<') || FindChar(Str, '>') || FindChar(Str, '|') || FindChar(Str, '+') || FindChar(Str, '.');
	if (ThatEnd) {
		result = result || FindChar(Str, ' ');
	}
	return !result;
}

/*���� ������ � ������*/
bool FindChar(string Str, char chr) {
	return Str.find(chr) != string::npos;
}

/*��������� ������ ������ ��� ���*/
bool StringEmpty(string Str) {
	if (Str.empty()) { return true; }
	string Str_ = Trim(Str);
	return Str_.empty();
}

/*�������� ���������� �� ����� game.json*/
string GetGameInfo(string ID) {
	string p = GetSessionInfo("GameJson");
	if (!HasDirectory(p)) { PF("game.json not found!", "C0007", true); return "ERROR_C0007"; }
	if (!JSONValid(p)) { PF("game.json corrupted!", "C0008", true); return "ERROR_C0008"; }
	return ReadJson(p, ID);
}

/*�������� ���������� �� ����� engine.json*/
string GetEngineInfo(string ID) {
	string p = GetSessionInfo("EngineJson");
	if (!HasDirectory(p)) { PF("engine.json not found!", "C0009", true); return "ERROR_C0009"; }
	if (!JSONValid(p)) { PF("engine.json corrupted!", "C0010", true); return "ERROR_C0010"; }
	return ReadJson(p, ID);
}

/*�������� ���������� �� ����� sessioninfo*/
string GetSessionInfo(string ID) {
	if (!HasDirectory(GamePath + SessionInfoPath)) { PF("Sessioninfo not found!","C0001", true); return "ERROR_C0001"; }
	if (!JSONValid(GamePath + SessionInfoPath)) { PF("Sessioninfo corrupted!","C0002", true); return "ERROR_C0002"; }
	return ReadJson(GamePath + SessionInfoPath,ID);
}

/*�������� ���������� � ����� sessioninfo*/
void SetSessionInfo(string ID, string Value) {
	WriteToJson(GamePath + SessionInfoPath, ID, Value);
}

/*������� ���������� � JSON ���� ��� �� ����������*/
void CreateValueJson(string Path, string ID, string DefaultValue) {
	if (ReadJson(Path, ID, "notfound") == "notfound") {
		WriteToJson(Path, ID, DefaultValue);
	}
}

/*�������� ���������� � JSON*/
void WriteToJson(string Path, string ID, string Value) {
	json data = json::parse(ReadFile(Path));
	data[ID] = Value;
	ofstream output_file(Path);
	output_file << data.dump(4);
}

/*�������� ���������� � JSON*/
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
		PE("JSON file [" + Path + "] has an incorrect structure!", "E0002");
		return "ERROR_E0002";
	}
}

/*��������� ������� ��������*/
string FillString(string Str, int Width, char Symbol, bool Invert) {
	if (Width <= 0) { return Str; }
	string Result = Str;
	int AddThat = Width - Str.size();
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

/*�������� ����� ������� ��������� ����*/
/*
   "s"  - ���������� �������
   "m"  - ���������� ������
   "h"  - ���������� ����
   "d"  - ���������� ���
   "mn" - ���������� ������
   "y"  - ���������� ����
   "w"  - ���������� ������
*/
string GetTimeComponent(time_t Time, string Tag) {
	struct tm Now;
	struct tm* NowPtr = &Now;
	int Val = -1;
	int AddZeros = 0;
	localtime_s(NowPtr, &Time);
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
	if (Val == -1) { PE("Tag ["+Tag+"] does not exist in the function GetTimeComponent()!","E0003"); return "ERROR_E0003"; }
	return FillString(to_string(Val), AddZeros, '0', true);
}

/*�������� ������� ����� ��������� ����*/
string GetTime(string Tag) {
	return GetTimeComponent(time(0),Tag);
}

/*�������� Value �� Map ��������� Key*/
template <typename K, typename V>
V GetFromMap(const map<K, V>& m, const K& key) {
	auto it = m.find(key);
	if (it != m.end()) {
		return it->second;
	}
	return V{};
}

/*������������ ������ � JSON ������*/
string ConvertToJSON(map<string, string> KeysValues) {
	string result = "{";
	for (auto Key_ = KeysValues.begin(); Key_ != KeysValues.end(); ++Key_) {
		string Key = Key_->first;
		string Val = GetFromMap(KeysValues, Key);
		result = result + "\"" + Key + "\": \""+Val+"\"";
		if (next(Key_) != KeysValues.end()) {
			result = result + ",";
		}
	}
	result = result + "}";
	json data = json::parse(result);
	return data.dump(4);
}

/*��������� �������� �� JSON ������*/
bool JSONValid(string Path) {
	return json::accept(ReadFile(Path));
}

/*�������� ������ �� �����*/
string ReadFile(string Path) {
	return GetFileInfo(Path);
}

/*������� ������ � ����*/
void WriteToFile(string Path, string Text, bool AddToNextLine) {
	if (AddToNextLine) {
		AddFileInfo(Path,Text);
	}
	else {
		SetFileInfo(Path,Text);
	}
}

/*�������� ������� � ������*/
string ReplaceString(string Str, string That = " ", string ToThat = "") {
	return regex_replace(Str,regex("\\"+That),ToThat);
}