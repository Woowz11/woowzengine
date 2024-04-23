#include <string>
#include <map>
#include <chrono>
#include <list>
#include <glad/gl.h>
#include "Vertex.h"
#include "time.h"
#pragma once
using namespace std;

string DoubleToString(double value);
float GetFractionalPart(int i);
string GetPathWithoutFileName(string path);
string FileName(string path);
bool SafeMode();
void SetRandomSeed(int seed);
float Random(float,float);
float Random();
void Exit();
void BaseInstall(string GamePath_);
void Print(string,int=8);
string ConvertTextToConsoleLogMessage(string Text, string Module = "UNKNOWN", char StartSymbol = ' ');
string Uppercase(string Str);
string Lowercase(string Str);
void Print(int,int=8);
bool FindChar(string Str, char chr);
bool NameWindowsAccept(string Str, bool ThatEnd = false, bool ThatPath = false);
void P(string Module = "UNKNOWN", string Text = "Default Message", int color=8);
void PP(string Text = "Default Print");
void PE(string Text = "Default Error",string ErrorCode = "0000");
void PW(string Text = "Default Warning",string Code = "W0000");
void PF(string Text = "Default Fatal",string Code = "C0000", bool DontPrint = false);
void Wait(int Milliseconds);
string FillString(string Str, int Width = 10, char Symbol = '#',bool Invert = false);
string GetTimeComponent(std::chrono::system_clock::time_point Time, string Tag = "s");
string GetTime(string Tag = "s");
bool GetOrCreateFolder(string);
bool GetOrCreateFile(string);
bool StringToBool(string Str);
bool JSONValid(string Path);
string ReadFile(string Path);
bool OnlyASCII(const std::string& str);
void CreateValueJson(string Path, string ID, string Value);
bool StringEmpty(string Str);
string ReadJson(string Path, string ID, string IfNotFound = "");
map<string, string> ReadAllJson(string Path);
string Trim(string s);
string GetFileName(string Path);
template <typename K, typename V>
V GetFromMapExtra(const map<K, V>& m, const K& key);
GLuint GetFromMap(map<string, GLuint> map, string id);
int GetFromMap(map<string, int> map, string id);
map<string, int> GetFromMap(map<string, map<string, int>> map, string id);
template <typename T>
T GetFromListExtra(const std::list<T>& myList, int index);
Vertex GetFromList(std::list<Vertex> list, int i);
void WriteToFile(string Path, string Text,bool = false);
void WriteToJson(string Path, string ID, string Value);
void SetSessionInfo(string ID, string Value);
string ConvertToJSON(map<string, string> KeysValues);
string GetSessionInfo(string ID);
string GetGameInfo(string ID);
string GetEngineInfo(string ID);
string GetSessionInfoIE(string ID);
string GetGameInfoIE(string ID);
string GetEngineInfoIE(string ID);
bool CheckPathToSymbols(string path);
void SetSessionInfo(string ID, string Value);
string ReplaceString(string,string,string);
bool StringHasString(string Str, string WhatNeedFound);