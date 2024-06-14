#pragma warning(disable : 4101)

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

unsigned char* StringToUnsignedChar(string str) {
	size_t size = str.size() + 1;
	unsigned char* ptr = new unsigned char[size];
	memcpy(ptr, str.c_str(), size);
	return ptr;
}

/*���������� ������ � ������ ��������*/
char* StringToCharArray(string& str) {
	char* charArray = new char[str.size() + 1];
	const char* cstr = StringToConstChar(str);
	std::copy(cstr, cstr + str.size() + 1, charArray);
	charArray[str.size()] = '\0';

	return charArray;
}

/*���������� ������ � LPCSTR*/
LPCSTR StringToLPCSTR(string str) {
	LPCSTR l = str.c_str();
	return l;
}

/*���������� const wchar_t � ������*/
string ConstWCharToString(const wchar_t* wchar) {
	wstring ws(wchar);
	return string(ws.begin(), ws.end());
}

/*���������� const char � const wchar_t*/
const wchar_t* ConstCharToConstWChar(const char* str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	std::vector<wchar_t> buffer(size_needed);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &buffer[0], size_needed);

	return &buffer[0];
}

/*���������� TCHART � string*/
string TCHARToString(const TCHAR* t) {
	wstring wstr(t);
	string str(wstr.begin(),wstr.end());
	return str;
}

/*���������� const unsigned char � ������*/
string ConstUnsignedCharToString(const unsigned char* chr) {
	return std::string(reinterpret_cast<const char*>(chr));
}

/*���������� ������ � ������*/
string CharToString(char c) {
	string s(1, c);
	return s;
}

/*���������� w������ � w������*/
wstring WCharToWString(wchar_t c) {
	wstring s(1, c);
	return s;
}

/*���������� ������� ����� � double*/
double FloatToDouble(float f) {
	return (double)f;
}

/*���������� ������ � ������� �����*/
int StringToInt(string Str) {
	float f = StringToFloat(Str, -627892216);
	if (f == -627892216) {
		PE("It is not possible to convert a string to a int! StringToInt('"+Str+"')","E0039");
		return -1;
	}
	return FloatToInt(f);
}

/*���������� ������ � ������� �����*/
float StringToFloat(string Str, float IfError) {
	if (IfError == -627892215) {
		float f = -1;
		try {
			f = stof(Str);
		}
		catch (const std::invalid_argument& e) {
			PE("It is not possible to convert a string to a float! StringToFloat('" + Str + "')", "E0005");
			return -1;
		}
		catch (const std::out_of_range& e) {
			PE("Can't convert a string to a float because it's huge! StringToFloat('" + Str + "')", "E0006");
			return -1;
		}
		return f;
	}
	else {
		float f = IfError;
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

/*���������� DWORD � �����*/
int DWORDToInt(DWORD D) {
	return static_cast<int>(D);
}

/*���������� ����� � DWORD*/
DWORD IntToDWORD(int i) {
	return (DWORD)i;
}

/*���������� ������� ����� � �����*/
int FloatToInt(float f) {
	return (int)floor(f);
}

/*���������� w������ � ������*/
string WStringToString(wstring wstr) {
	if (wstr.empty()) {
		return std::string();
	}

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &strTo[0], size_needed, NULL, NULL);

	return strTo;
}

/*���������� ������ � w������*/
wstring StringToWString(string Str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);
	wstring converted_str(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &converted_str[0], size_needed);
	return converted_str;
}

/*���������� ������ � LPWSTR*/
LPWSTR StringToLPWSTR(string str) {
	int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
	LPWSTR wide_string = new WCHAR[size_needed + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), wide_string, size_needed);
	wide_string[size_needed] = 0;

	return wide_string;
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

/*���������� LPVOID � ������*/
string LPVOIDToString(LPVOID lpData) {
	char* pData = reinterpret_cast<char*>(lpData);
	std::string strData(pData);
	return strData;
}

/*���������� ������ � LPCTSTR*/
LPCTSTR StringToLPCTSTR(string str) {
	return (LPCTSTR)StringToConstChar(str);
}

/*���������� ������ � PVOID*/
PVOID StringToPVOID(string str) {
	return (PVOID)(str.c_str());
}

/*���������� ������ � ���� ( �������� "\" �� "/" )*/
string StringToPath(string Str) {
	return ReplaceString(Str, "\\", "/");
}