#include <iostream>
#include <string_view>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

using namespace std;

int main(int argc, char** argv)
{
    cout << "Woowzengine realization..." << endl;

    string GamePath = "F:\\woowzengine\\example_game\\";
    if (argv[1] != NULL) {
        GamePath = string(argv[1]);
    }

    string result = "Game opening: " + GamePath;
    cout << result << endl;
    system("pause");
    return 0;
}