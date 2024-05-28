#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <sys/stat.h>
#include <direct.h>
#include <filesystem>
#include <map>
#include <fstream>
#include "Files.h"
#include "Easyer.h"
#include "Base.h"
#include "OpenGame.h"

using namespace std;
namespace fs = std::filesystem;

/*Проверяет директорию или файл на существование*/
bool HasDirectory(string Dir) {
	if (CheckPathToSymbols(Dir)) {
		struct stat sb;
		return stat(StringToConstChar(StringToPath(Dir)), &sb) == 0;
	}
	else {
		return false;
	}
}

map<string, string> GetFilesFromDirectory(string Dir) {
	Dir = StringToPath(Dir);
	map<string, string> result = {};
	for (const auto& entry : fs::directory_iterator(Dir)) {
		string path = StringToPath(entry.path().string());
		result[GetFileName(path)] = path;
	}

	return result;
}

/*Получить тип файла*/
string GetFileType(string dir) {
	size_t pos = dir.find_first_of(".");
	if (pos != std::string::npos) {
		return dir.substr(pos + 1);
	}
	return "";
}


/*Создаёт директорию по пути*/
bool CreateDirectory(string Dir) {
	if (CheckPathToSymbols(Dir)) {
		if (HasDirectory(Dir) == false) {
			_mkdir(StringToConstChar(Dir + "/"));
			return true;
		}
		return false;
	}
	else { return false; }
}

/*Создаёт файл по пути*/
bool CreateFile(string Dir) {
	if (CheckPathToSymbols(Dir)) {
		if (HasDirectory(Dir) == false) {
			ofstream outfile(Dir);
			outfile.close();
			return true;
		}
		return false;
	}
	else {
		return false;
	}
}

/*Вписать данные в файл*/
void SetFileInfo(string Dir, string Info) {
	if (CheckPathToSymbols(Dir)) {
		if (HasDirectory(Dir) == true) {
			ofstream file;
			file.open(Dir, ios::out);
			file << Info << "\n";
			file.close();
		}
	}
}

/*Вписать данные в файл, на следущей строчке*/
void AddFileInfo(string Dir, string Info) {
	if (CheckPathToSymbols(Dir)) {
		if (HasDirectory(Dir) == true) {
			string result = GetFileInfo(Dir) + Info;
			SetFileInfo(Dir, result);
		}
	}
}

/*Переминовать файл*/
void RenameFile(string Dir, string DirAndNewName) {
	if (CheckPathToSymbols(Dir)) {
		if (HasDirectory(Dir) == true) {
			rename(StringToConstChar(Dir), StringToConstChar(DirAndNewName));
		}
		else {
			PE("File not found to rename! RenameFile('"+Dir+"','"+DirAndNewName+"')", "F0000");
		}
	}
}

/*Получить данные из файла*/
string GetFileInfo(string Dir) {
	if (CheckPathToSymbols(Dir)) {
		if (HasDirectory(Dir) == false) { PE("File was not found! When trying to read it. GetFileInfo('"+Dir+"')", "F0001"); return "ERROR_F0001"; }
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
			PE("File was not opened! When trying to read it. GetFileInfo('" + Dir + "')", "F0002");
		}

		return result;
	}
	else {
		return "E0010";
	}
}

void RemoveFile(string path) {
	std::FILE* file;
	fopen_s(&file, StringToConstChar(path), "r");
	std::fclose(file);
	if (std::remove(StringToConstChar(path)) != 0) {
		PE("Couldn't delete the file! RemoveFile('"+path+"')", "F0003");
	}
}