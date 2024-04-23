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

/*Отправка сообщения в консоль*/
int PrintToConsole(string Text, int Color) {
	setlocale(LC_ALL, "");
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/*Установка цвета в консоли*/
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