#include <string>
#include <map>
#pragma once
using namespace std;

void RenameFile(string Dir, string DirAndNewName);
bool HasDirectory(string Dir);
bool CreateDirectory(string Dir);
bool CreateFile(string Dir);
void AddFileInfo(string Dir, string Info);
void SetFileInfo(string Dir, string Info);
string GetFileInfo(string Dir);
map<string, string> GetFilesFromDirectory(string Dir);
string GetFileType(string dir);
void RemoveFile(string path);