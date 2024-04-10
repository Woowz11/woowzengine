#include <iostream>
#include <filesystem>
#include <regex>
#include "Base.h"
#include "Console.h"
#include "Easyer.h"
#include "Files.h"

using namespace std;

#pragma region ConsoleZone
/*Отправка сообщения в консоль и остальное*/
void Print(string Text) {
	PrintToConsole(string(Text));
}
void Print(int Text) {
	Print(to_string(Text));
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

/*Получить информацию из файла sessioninfo.json*/
string GetSessionInfo(string ID) {
	return "";
}

/*Заменить информацию в файле sessioninfo.json*/
void SetSessionInfo(string ID, string Value) {

}

/*Получить данные из файла*/
string ReadFile(string Path) {
	return GetFileInfo(Path);
}

/*Вписать данные в файл*/
void WriteToFile(string Path, string Text, bool AddToNextLine = false) {
	if (AddToNextLine) {
		AddFileInfo(Path,Text);
	}
	else {
		SetFileInfo(Path,Text);
	}
}

/*Заменяет символы в строке*/
string ReplaceString(string Str, string That = " ", string ToThat = "") {
	return regex_replace(Str,regex("\\"+That),ToThat);
}