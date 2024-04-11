#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <sys/stat.h>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include "Files.h"
#include "Easyer.h"
#include "Base.h"

using namespace std;

/*Проверяет директорию или файл на существование*/
bool HasDirectory(string Dir) {
	struct stat sb;
	return stat(StringToConstChar(StringToPath(Dir)), &sb) == 0;
}

/*Создаёт директорию по пути*/
bool CreateDirectory(string Dir) {
	if (HasDirectory(Dir) == false) {
		_mkdir(StringToConstChar(Dir + "/"));
		return true;
	}
	return false;
}

/*Создаёт файл по пути*/
bool CreateFile(string Dir) {
	if (HasDirectory(Dir) == false) {
		ofstream outfile(Dir);
		outfile.close();
		return true;
	}
	return false;
}

/*Вписать данные в файл*/
void SetFileInfo(string Dir, string Info) {
	if (HasDirectory(Dir) == true) {
		ofstream file;
		file.open(Dir,ios::out);
		file << Info << "\n";
		file.close();
	}
}

/*Вписать данные в файл, на следущей строчке*/
void AddFileInfo(string Dir, string Info) {
	if (HasDirectory(Dir) == true) {
		string result = GetFileInfo(Dir) + Info;
		SetFileInfo(Dir,result);
	}
}

/*Переминовать файл*/
void RenameFile(string Dir, string DirAndNewName) {
	if (HasDirectory(Dir) == true) {
		rename(StringToConstChar(Dir), StringToConstChar(DirAndNewName));
	}
	else {
		PE("File [" + Dir + "] not found to rename!", "F0000");
	}
}

/*Получить данные из файла*/
string GetFileInfo(string Dir) {
	if (HasDirectory(Dir) == false) { PE("File [" + Dir + "] was not found! When trying to read it.", "F0001"); return "ERROR_F0001"; }
	ifstream file;
	file.open(Dir);
	string result = "ERROR_F0002";
	string line;
	if (file.is_open()) {
		result = "";
		while (getline(file, line)) {
			result = result + line + "\n";
		}
		file.close();
	}
	else {
		PE("File was not opened! When trying to read it.","F0002");
	}

	return result;
}