#include <iostream>
#include <string>
#include <string_view>
#include "OpenGame.h"

using namespace std;

void StartEngine(string GamePath) {
    OpenGame(GamePath);
}

int main(int argc, char** argv)
{
    cout << "Woowzengine realization..." << "\n";

    string GamePath = "F:\\woowzengine\\example_game\\";
    if (argv[1] != NULL) {
        GamePath = string(argv[1]);
    }

    cout << "Game opening: " << GamePath << "\n";
    StartEngine(GamePath);
    system("pause");
    return EXIT_SUCCESS;
}