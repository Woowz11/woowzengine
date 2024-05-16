#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "Console.h"
#include "Easyer.h"
#include "fcntl.h"
#include "io.h"

using namespace std;

int DefaultConsoleColor = 8;

void ConsoleInstall() {
    HWND console = GetConsoleWindow();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

    wcscpy_s(fontInfo.FaceName, L"Consolas");
    fontInfo.dwFontSize.X = 12;
    fontInfo.dwFontSize.Y = 16;

    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

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

	_setmode(_fileno(stdout), _O_U16TEXT);
	wcout << StringToWString(Text) << "\n";
	SetConsoleTextAttribute(hConsole, DefaultConsoleColor);
	return 0;
}