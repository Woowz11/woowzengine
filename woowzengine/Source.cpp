#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <string_view>
#include "OpenGame.h"
#include "Easyer.h"
#include "Files.h"
#include "WindowsElements.h"
#include "base.h"
#include "Console.h"
#include "GLFW.h"

using namespace std;

string EngineVersion = "0.0.2a";
bool DebugVersion = false;

void StartEngine(string GamePath) {
    OpenGame(GamePath, EngineVersion, DebugVersion);
}

int main(int argc, char** argv)
{
    if (!DebugVersion) { ::ShowWindow(::GetConsoleWindow(), SW_HIDE); }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    ConsoleInstall();
    SetConsoleTitle(StringToLPCWSTR("WoowzEngine Console"));
    cout << "WoowzEngine realization..." << "\n";
    if (DebugVersion) { cout << "Debug mode enabled!" << "\n"; }

    string GamePath = "F:\\woowzengine\\example_game\\";
    if (argv[1] != NULL) {
        GamePath = string(argv[1]);
    }
    else {
        if (!HasDirectory(StringToPath(GamePath))) {
            MessageBoxFatal("So far the engine does not support opening games through a dialog box, stick this exe file in an empty folder and run .bat file!\nLook https://woowz11.github.io/woowzsite/woowzengine.html","0",true);
        }
    }

    cout << "Engine version: " << EngineVersion << "\n";
    cout << "Game opening: " << GamePath << "\n";
    cout << "[]==============[Log]==============[]" << "\n";
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)StopEngine, true);
    StartEngine(GamePath);
    std::cin.get();
    return EXIT_SUCCESS;
}