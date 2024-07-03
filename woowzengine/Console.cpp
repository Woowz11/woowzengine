#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <locale>
#include <codecvt>
#include <sol/sol.hpp>
#include "Console.h"
#include "fcntl.h"
#include "io.h"
#include "Cycles.h"
#include "Easyer.h"

using namespace std;

sol::function PrintFunction;
HANDLE hConsole;

void SetPrintFunction(sol::function f) {
	PrintFunction = f;
}

bool RunPrintFunction(string text, string type, string module, string code, int color, string c) {
	if (PrintFunction.valid()) {
		sol::object obj = PrintFunction(text,type,module,code,color,c);
		sol::type type = obj.get_type();

		switch (type) {
		case sol::type::nil:
			return true;
			break;
		case sol::type::boolean:
			return obj.as<bool>();
			break;
		}
		return false;
	}
	return true;
}

void ConsoleInstall() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	wcscpy_s(fontInfo.FaceName, L"NSimSun");
	fontInfo.dwFontSize.X = 8;
	fontInfo.dwFontSize.Y = 16;

    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1000, 500, TRUE);
}

/*Отправка сообщения в консоль*/
int PrintToConsole(string Text, int Color) {
	/*Установка цвета в консоли*/
	if (Color != -1) {
		SetConsoleTextAttribute(hConsole, Color);
	}
	else {
		SetConsoleTextAttribute(hConsole, 8);
	}

	_setmode(_fileno(stdout), _O_U16TEXT);
	std::wstring wideText = StringToWString(Text);
	wcout << wideText << L"\n";

	return 0;
}

