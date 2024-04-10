#include <string>
#pragma once
using namespace std;

bool HasDirectory(string Dir);
bool CreateDirectory(string Dir);
bool CreateFile(string Dir);
void AddFileInfo(string Dir, string Info);
void SetFileInfo(string Dir, string Info);
string GetFileInfo(string Dir);