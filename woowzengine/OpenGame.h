#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <string>
#include <sol/sol.hpp>
#pragma once
using namespace std;

void DebugPrint(string text);
void SetGameClosedEvent(sol::function f);
bool WINAPI StopEngine(DWORD CEvent);
void CheckFiles(string);
void Install(string);
void Cycle();
void GameInstall();
void OpenGame(string, string, bool);