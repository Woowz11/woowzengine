#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "Console.h"
#include "Easyer.h"

using namespace std;

int DefaultConsoleColor = 8;

/*�������� ��������� � �������*/
int PrintToConsole(string Text, int Color) {
	setlocale(LC_ALL, "");
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/*��������� ����� � �������*/
	if (Color != -1) {
		SetConsoleTextAttribute(hConsole, Color);
	}
	else {
		SetConsoleTextAttribute(hConsole, DefaultConsoleColor);
	}

	wcout << StringToWString(Text) << "\n";
	SetConsoleTextAttribute(hConsole, DefaultConsoleColor);
	return 0;
}