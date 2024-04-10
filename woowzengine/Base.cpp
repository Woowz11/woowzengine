#include <iostream>
#include <filesystem>
#include <regex>
#include "Base.h"
#include "Console.h"
#include "Easyer.h"
#include "Files.h"

using namespace std;

#pragma region ConsoleZone
/*�������� ��������� � ������� � ���������*/
void Print(string Text) {
	PrintToConsole(string(Text));
}
void Print(int Text) {
	Print(to_string(Text));
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

/*�������� ���������� �� ����� sessioninfo.json*/
string GetSessionInfo(string ID) {
	return "";
}

/*�������� ���������� � ����� sessioninfo.json*/
void SetSessionInfo(string ID, string Value) {

}

/*�������� ������ �� �����*/
string ReadFile(string Path) {
	return GetFileInfo(Path);
}

/*������� ������ � ����*/
void WriteToFile(string Path, string Text, bool AddToNextLine = false) {
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