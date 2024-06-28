#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <locale>
#include <algorithm>
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

string EngineVersion = "0.0.3";
bool DebugVersion = false;

string EngineName = "woowzengine.exe";
string WoowzEnginePosition = "F:\\woowzengine\\WoowzEngine\\Build\\woowzengine\\Release\\"+EngineName;
string WoowzGamePosition = "F:\\woowzengine\\example_game\\"+EngineName;

int main(int argc, char** argv)
{
    ::ShowWindow(::GetConsoleWindow(), (DebugVersion? SW_SHOW : SW_HIDE));
    if (argv[0] == WoowzEnginePosition) {
        argv[0] = StringToCharArray(WoowzGamePosition);
    }

    if (argv[0][0] != '"') {
        argv[0] = StringToCharArray("\"" + string(argv[0]) + "\"");
    }

    string args_ = "";
    for (int i = 0; i < argc; i++) {
        args_ = args_ + " " + argv[i];
    }
    args_ = args_.substr(1);

    if (argc == 1) {
        args_ = string(argv[0]) + " run";
    }

    string PathToExe = "";

    size_t first_quote = args_.find_first_of("\"");
    if (first_quote != string::npos) {
        size_t second_quote = args_.find_first_of("\"", first_quote + 1);
        if (second_quote != string::npos) {
            PathToExe = args_.substr(first_quote + 1, second_quote - first_quote - 1);
        }
    }

    args_ = ReplaceString(args_, "\"" + PathToExe + "\" ", "");
    vector<string> args = StringSplit(args_, ' ');

    string PathToProject = ReplaceString(PathToExe, "\\"+EngineName, "");

    /*======================================================================*/

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    ConsoleInstall();
    SetConsoleTitle_("WoowzEngine Console");
    cout << "WoowzEngine realization..." << endl;
    cout << "Engine path: " << PathToExe << endl;
    cout << "Engine version: " << EngineVersion << endl;
    if (DebugVersion) { cout << "Debug mode enabled!" << endl; }
    if (!SupportedWindowsVersion()) {
        cout << "Unsupported version of Windows! Possible errors. [Supported 10 and 11]" << "\n";
    }
    if (!CheckInternet()) {
        cout << "Engine without internet! Possible errors." << "\n";
    }

    bool HasError = false;
    if (args.size() >= 1) {
        if (args[0] == "run") {
            cout << "Running project: "+PathToProject << endl;

            if (args[1] == "debug") {
                DebugVersion = true;
                cout << "Debug mode enabled from run.bat!" << endl;
            }

            cout << "[]==============[Log]==============[]" << "\n";
            SetConsoleCtrlHandler((PHANDLER_ROUTINE)StopEngine, true);
            OpenGame(StringToPath(PathToProject + "\\"), EngineVersion, DebugVersion, EngineName);

        }
        else {
            HasError = true;
        }
        
    }
    else {
        HasError = true;
    }

    if (HasError) {
        MessageBoxFatal("Error in run.bat file! Notify Woowz11!", "OTHER", true);
    }

    if (DebugVersion) { std::cin.get(); }
    return EXIT_SUCCESS;
}