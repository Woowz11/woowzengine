#include <iostream>
#include <string>
#include <string_view>
#include <windows.h>
#include "OpenGame.h"
#include "Easyer.h"
#include "Files.h"
#include "WindowsElements.h"

using namespace std;

string EngineVersion = "0.0.1a";

void StartEngine(string GamePath) {
    OpenGame(GamePath, EngineVersion);
}

int main(int argc, char** argv)
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    SetConsoleTitle(StringToLPCWSTR("WoowzEngine Console"));
    cout << "WoowzEngine realization..." << "\n";

    string GamePath = "F:\\woowzengine\\example_game\\";
    if (argv[1] != NULL) {
        GamePath = string(argv[1]);
    }
    else {
        if (!HasDirectory(StringToPath(GamePath))) {
            MessageBoxFatal("So far the engine does not support opening games through a business window, stick this exe file in an empty folder and run .bat file!","0",true);
        }
    }

    cout << "Engine version: " << EngineVersion << "\n";
    cout << "Game opening: " << GamePath << "\n";
    cout << "[]==============[Log]==============[]" << "\n";
    StartEngine(GamePath);
    system("pause");
    return EXIT_SUCCESS;
}