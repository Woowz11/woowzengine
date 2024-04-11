#include <iostream>
#include "Windows.h"
#pragma once
using namespace std;

DWORD IntToDWORD(int i);
int FloatToInt(float f);
wstring StringToWString(string Str);
const char* StringToConstChar(string);
string StringToPath(string);
LPCWSTR WStringToLPCWSTR(wstring str);
LPCWSTR ConstCharToLPCWSTR(const char* charString);
LPCWSTR StringToLPCWSTR(string Str);