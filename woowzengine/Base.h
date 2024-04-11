#include <string>
#include <map>
#include "time.h"
#pragma once
using namespace std;

void Exit();
void BaseInstall(string GamePath_);
void Print(string,int=-1);
string ConvertTextToConsoleLogMessage(string Text, string Module = "UNKNOWN", char StartSymbol = ' ');
string Uppercase(string Str);
string Lowercase(string Str);
void Print(int);
bool FindChar(string Str, char chr);
bool NameWindowsAccept(string Str, bool ThatEnd = false);
void P(string Module = "UNKNOWN", string Text = "Default Message");
void PP(string Text = "Default Print");
void PE(string Text = "Default Error",string ErrorCode = "0000");
void PW(string Text = "Default Warning",string Code = "W0000");
void PF(string Text = "Default Fatal",string Code = "C0000", bool DontPrint = false);
void Wait(float Seconds);
string FillString(string Str, int Width = 10, char Symbol = '#',bool Invert = false);
string GetTimeComponent(time_t Time, string Tag = "s");
string GetTime(string Tag = "s");
bool GetOrCreateFolder(string);
bool GetOrCreateFile(string);
bool StringToBool(string Str);
bool JSONValid(string Path);
string ReadFile(string Path);
void CreateValueJson(string Path, string ID, string Value);
bool StringEmpty(string Str);
string ReadJson(string Path, string ID, string IfNotFound = "");
string Trim(string s);
template <typename K, typename V>
V GetFromMap(const map<K, V>& m, const K& key);
void WriteToFile(string Path, string Text,bool = false);
void WriteToJson(string Path, string ID, string Value);
void SetSessionInfo(string ID, string Value);
string ConvertToJSON(map<string, string> KeysValues);
string GetSessionInfo(string ID);
string GetGameInfo(string ID);
string GetEngineInfo(string ID);
void SetSessionInfo(string ID, string Value);
string ReplaceString(string,string,string);
bool StringHasString(string Str, string WhatNeedFound);