#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <vector>
#include "Easyer.h"
#include "Base.h"

using namespace std;

/*���������� ������� ����� � double*/
double FloatToDouble(float f) {
	return (double)f;
}

/*���������� ������ � ������� �����*/
int StringToInt(string Str) {
	return FloatToInt(StringToFloat(Str));
}

/*���������� ������ � ������� �����*/
float StringToFloat(string Str) {
	return stof(Str);
}

/*���������� ����� � DWORD*/
DWORD IntToDWORD(int i) {
	return (DWORD)i;
}

/*���������� ������� ����� � �����*/
int FloatToInt(float f) {
	return (int)round(f);
}

/*���������� ������ � w������*/
wstring StringToWString(string Str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);
	wstring converted_str(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &converted_str[0], size_needed);
	return converted_str;
}

/*���������� const char � LCPWSTR*/
LPCWSTR ConstCharToLPCWSTR(const char* charString) {
	string s = charString;
	return StringToLPCWSTR(s);
}

/*���������� const char � LCPWSTR*/
LPCWSTR StringToLPCWSTR(string Str) {
	return WStringToLPCWSTR(StringToWString(Str));
}

/*���������� const char � LCPWSTR*/
LPCWSTR WStringToLPCWSTR(wstring Str) {
	static std::vector<wchar_t> buffer;
	buffer.assign(Str.begin(), Str.end());
	buffer.push_back(L'\0');
	return buffer.data();
}

/*���������� ������ � const char*/
const char* StringToConstChar(string Str) {
	char* result = new char[Str.length() + 1];
	for (int i = 0; i < Str.length(); i++) {
		result[i] = Str[i];
	}
	result[Str.length()] = '\0';
	return result;
}

/*���������� ������ � ���� ( �������� "\" �� "/" )*/
string StringToPath(string Str) {
	return ReplaceString(Str, "\\", "/");
}