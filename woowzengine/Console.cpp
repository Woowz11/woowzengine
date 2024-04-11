#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include "Console.h"

using namespace std;

int DefaultConsoleColor = 8;

/*Отправка сообщения в консоль*/
int PrintToConsole(string Text, int Color) {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Color != -1) {
		SetConsoleTextAttribute(hConsole, Color);
	}
	else {
		SetConsoleTextAttribute(hConsole, DefaultConsoleColor);
	}
	cout << Text << "\n";
	SetConsoleTextAttribute(hConsole, DefaultConsoleColor);
	return 0;
}