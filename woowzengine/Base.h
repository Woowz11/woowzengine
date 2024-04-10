#include <string>
#pragma once
using namespace std;

void Print(string);
void Print(int);
bool GetOrCreateFolder(string);
bool GetOrCreateFile(string);
string ReadFile(string Path);
void WriteToFile(string Path, string Text,bool);
string GetSessionInfo(string ID);
void SetSessionInfo(string ID, string Value);
string ReplaceString(string,string,string);