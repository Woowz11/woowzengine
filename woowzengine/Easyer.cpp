#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include "Easyer.h"
#include "Base.h"

using namespace std;

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