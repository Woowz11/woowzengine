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
#include <list>
#include "Easyer.h"
#include "Base.h"

using namespace std;

/*���������� const unsigned char � ������*/
string ConstUnsignedCharToString(const unsigned char* chr) {
	return std::string(reinterpret_cast<const char*>(chr));
}

/*���������� ������ � ������*/
string CharToString(char c) {
	string s(1, c);
	return s;
}

/*���������� ������� ����� � double*/
double FloatToDouble(float f) {
	return (double)f;
}

/*���������� ������ � ������� �����*/
int StringToInt(string Str) {
	return FloatToInt(StringToFloat(Str));
}

/*���������� ������ � ������� �����*/
float StringToFloat(string Str, float IfError) {
	if (IfError == -627892215) {
		float f = -1;
		try {
			f = stof(Str);
		}
		catch (const std::invalid_argument& e) {
			PE("It is not possible to convert a string to a number! StringToFloat('" + Str + "')", "E0005");
			return -1;
		}
		catch (const std::out_of_range& e) {
			PE("Can't convert a string to a number because it's huge! StringToFloat('" + Str + "')", "E0006");
			return -1;
		}
		return f;
	}
	else {
		float f = -1;
		try {
			f = stof(Str);
		}
		catch (const std::invalid_argument& e) {
			return IfError;
		}
		catch (const std::out_of_range& e) {
			return IfError;
		}
		return f;
	}
}

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