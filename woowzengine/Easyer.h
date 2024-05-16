#pragma once
#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <list>
using namespace std;

string ConstUnsignedCharToString(const unsigned char* chr);
string CharToString(char c);
double FloatToDouble(float f);
int StringToInt(string Str);
float StringToFloat(string Str, float IfError = -627892215);
DWORD IntToDWORD(int i);
int FloatToInt(float f);
wstring StringToWString(string Str);
const char* StringToConstChar(string);
string StringToPath(string);
LPCWSTR WStringToLPCWSTR(wstring str);
LPCWSTR ConstCharToLPCWSTR(const char* charString);
LPCWSTR StringToLPCWSTR(string Str);