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

/*Превращает число в DWORD*/
DWORD IntToDWORD(int i) {
	return (DWORD)i;
}

/*Превращает дробное число в число*/
int FloatToInt(float f) {
	return (int)floor(f);
}

/*Превращает строку в wстроку*/
wstring StringToWString(string Str) {
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(Str);
}

/*Превращает const char в LCPWSTR*/
LPCWSTR ConstCharToLPCWSTR(const char* charString) {
	string s = charString;
	return StringToLPCWSTR(s);
}

/*Превращает const char в LCPWSTR*/
LPCWSTR StringToLPCWSTR(string Str) {
	return WStringToLPCWSTR(StringToWString(Str));
}

/*Превращает const char в LCPWSTR*/
LPCWSTR WStringToLPCWSTR(wstring Str) {
	static std::vector<wchar_t> buffer;
	buffer.assign(Str.begin(), Str.end());
	buffer.push_back(L'\0');
	return buffer.data();
}

/*Превращает строку в const char*/
const char* StringToConstChar(string Str) {
	char* result = new char[Str.length()];
	for (int i = 0; i < Str.length(); i++) {
		result[i] = Str[i];
	}
	result[Str.length()] = '\0';
	return result;
}

/*Превращает строку в путь ( заменяет "\" на "/" )*/
string StringToPath(string Str) {
	return ReplaceString(Str, "\\", "/");
}