#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include <filesystem>
#include "Base.h"
#include "Easyer.h"

using namespace std;

/*Зона игры*/

/*Проверяет работает ли lua или нет*/
void CheckLua() {
	P("LUACHECK", "Lua supported!");
}

/*Зона woowzengine*/

void LuaCompile() {
	sol::state lua{};
	lua.open_libraries(sol::lib::package);

	/*Загрузка скриптов из игры*/
	const string package_path_ = lua["package"]["path"];
	lua["package"]["path"] = package_path_ + (!package_path_.empty() ? ";" : "") + GetSessionInfo("SourcePath") + "?.lua";
	for (auto& p : std::filesystem::recursive_directory_iterator(GetSessionInfo("SourcePath"))) {
		if (p.is_directory()) {
			const string package_path = lua["package"]["path"];
			lua["package"]["path"] = package_path + (!package_path.empty() ? ";" : "") + p.path().string() + "/?.lua";
		}
	}
	CreateValueJson(GetSessionInfo("SessionPath"), "LuaLibs", lua["package"]["path"]);

	lua.set_function("CheckLua", &CheckLua);

	try {
		lua.script_file(GetSessionInfo("SourcePath") + "start.lua");
	}
	catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
		string what = e.what();
		PE(what, "LUA");
	}
}