#include <string>
#pragma once
using namespace std;

int PrintToConsole(string,int = -1);
void ConsoleInstall();
void SetPrintFunction(sol::function f);
bool RunPrintFunction(string text, string type, string module, string code, int color, string c);