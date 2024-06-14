#include <string>
#include <unordered_map>
using namespace std;

unordered_map<string, string> values;

/*
* ["Debug"]         - Включен ли режим отладки - "false"
* ["EngineJson"]    - Путь до engine.json - "F:/woowzengine/example_game/game/engine.json"
* ["GameJson"]      - Путь до game.json - "F:/woowzengine/example_game/game/game.json"
* ["GamePath"]      - Путь до игры - "F:/woowzengine/example_game/"
* ["Log"]           - Путь до свежего лога - "F:/woowzengine/example_game/woowzengine/log/2024-06-13-18-41-27-382.log"
* ["LuaLibs"]       - Файлы используемые в Lua - "F:\\woowzengine\\WoowzEngine\\Build\\woowzengine\\Release\\lua\\?.lua;F:\\woowzengine\\WoowzEngine\\Build\\woowzengine\\Release\\lua\\?\\init.lua;F:\\woowzengine\\..."
* ["MainWindow"]    - Выбраное главное окно - ""
* ["Seed"]          - Текущий случайный сид - "1718293287"
* ["SettingsJson"]  - Путь до settings.json - "F:/woowzengine/example_game/game/settings.json"
* ["SourcePath"]    - Путь до папки game - "F:/woowzengine/example_game/game/"
* ["TemporaryPath"] - Путь до папки temporary - "F:/woowzengine/example_game/woowzengine/temporary"
* ["Version"]       - Версия движка - "0.0.2e"
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