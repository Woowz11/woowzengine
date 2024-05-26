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
#include "Cycles.h"
#include "GLFW.h"

using namespace std;

string EngineVersion = "0.0.2d";
bool DebugVersion = false;

void StartEngine(string GamePath) {
    OpenGame(GamePath, EngineVersion, DebugVersion);
}

int main(int argc, char** argv)
{
    auto PathToExe = argv[0];
    if (!DebugVersion) { ::ShowWindow(::GetConsoleWindow(), SW_HIDE); }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    ConsoleInstall();
    SetConsoleTitle_("WoowzEngine Console");
    cout << "WoowzEngine realization..." << "\n";
    cout << "Engine path: " << PathToExe << "\n";
    cout << "Engine version: " << EngineVersion << "\n";
    if (DebugVersion) { cout << "Debug mode enabled!" << "\n"; }

    if (argc > 1) {
        if (string(argv[1]) != "run") {
            MessageBoxFatal("Error in run.bat file! Notify Woowz11!", "OTHER", true);
        }
    }
    else {
        argc = 2;
        argv = new char*[argc];
        argv[0] = PathToExe;
        argv[1] = "run";
    }

    string GamePath = "F:\\woowzengine\\example_game\\";
    if (argc > 2) {
        GamePath = "";
        int i = 2;
        while (argv[i] != nullptr) {
            GamePath = GamePath + string(argv[i]) + " ";
            i++;
        }
        GamePath = GamePath.substr(0, GamePath.length() - 1);
        GamePath = GamePath + "\\";
    }
    else {
        if (!HasDirectory(StringToPath(GamePath))) {
            MessageBoxFatal("So far the engine does not support opening games through a dialog box, stick this exe file in an empty folder and run.bat file!\nLook https://woowz11.github.io/woowzsite/woowzengine.html","C0019",true);
        }
    }

    cout << "Game path: " << GamePath << "\n";
    if (!SupportedWindowsVersion()) {
        cout << "Unsupported version of Windows! Possible errors. [Supported 10, 11]" << "\n";
    }
    if (!CheckInternet()) {
        cout << "Engine without internet!" << "\n";
    }
    cout << "[]==============[Log]==============[]" << "\n";
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)StopEngine, true);
    StartEngine(StringToPath(GamePath));
    std::cin.get();
    return EXIT_SUCCESS;
}