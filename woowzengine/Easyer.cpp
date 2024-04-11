#include <iostream>
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <vector>
#include "Easyer.h"
#include "Windows.h"
#include "Base.h"

using namespace std;

/*���������� ����� � DWORD*/
DWORD IntToDWORD(int i) {
	return (DWORD)i;
}

/*���������� ������� ����� � �����*/
int FloatToInt(float f) {
	return (int)floor(f);
}

/*���������� ������ � w������*/
wstring StringToWString(string Str) {
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(Str);
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
	char* result = new char[Str.length()];
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