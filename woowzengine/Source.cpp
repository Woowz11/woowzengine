#include <iostream>
#include <string>
#include <string_view>
#include <windows.h>
#include "OpenGame.h"
#include "Easyer.h"

using namespace std;

string EngineVersion = "0.0.1";

void StartEngine(string GamePath) {
    OpenGame(GamePath, EngineVersion);
}

int main(int argc, char** argv)
{
    SetConsoleTitle(StringToLPCWSTR("WoowzEngine Console"));
    cout << "WoowzEngine realization..." << "\n";

    string GamePath = "F:\\woowzengine\\example_game\\";
    if (argv[1] != NULL) {
        GamePath = string(argv[1]);
    }

    cout << "Engine version: " << EngineVersion << "\n";
    cout << "Game opening: " << GamePath << "\n";
    cout << "[]==============[Log]==============[]" << "\n";
    StartEngine(GamePath);
    system("pause");
    return EXIT_SUCCESS;
}