#include <string>
#include <unordered_map>
using namespace std;

unordered_map<string, string> values;

/*
* ["Debug"]         - ������� �� ����� ������� - "false"
* ["EngineJson"]    - ���� �� engine.json - "F:/woowzengine/example_game/game/engine.json"
* ["GameJson"]      - ���� �� game.json - "F:/woowzengine/example_game/game/game.json"
* ["GamePath"]      - ���� �� ���� - "F:/woowzengine/example_game/"
* ["Log"]           - ���� �� ������� ���� - "F:/woowzengine/example_game/woowzengine/log/2024-06-13-18-41-27-382.log"
* ["LuaLibs"]       - ����� ������������ � Lua - "F:\\woowzengine\\WoowzEngine\\Build\\woowzengine\\Release\\lua\\?.lua;F:\\woowzengine\\WoowzEngine\\Build\\woowzengine\\Release\\lua\\?\\init.lua;F:\\woowzengine\\..."
* ["MainWindow"]    - �������� ������� ���� - ""
* ["Seed"]          - ������� ��������� ��� - "1718293287"
* ["SettingsJson"]  - ���� �� settings.json - "F:/woowzengine/example_game/game/settings.json"
* ["SourcePath"]    - ���� �� ����� game - "F:/woowzengine/example_game/game/"
* ["TemporaryPath"] - ���� �� ����� temporary - "F:/woowzengine/example_game/woowzengine/temporary"
* ["Version"]       - ������ ������ - "0.0.2e"
*/

string GetSessionInfoData(string id) {
    auto it = values.find(id);
    if (it != values.end()) {
        return it->second;
    }
    else {
        return "SessionInfo data not found! [" + id + "]";
    }
}

void SetSessionInfoData(string id, string data) {
	values[id] = data;
}