#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include "OpenGame.h"
#include "Base.h"
#include "Files.h"
#include "Logger.h"
#include "Easyer.h"
#include "LuaCompile.h"
#include "GLFW.h"

using namespace std;

string SGamePath = "";
bool SessionInfoBroken = false;
bool JGameBroken = false;
bool JEngineBroken = false;
bool HasStartScript = true;

void CheckFiles(string ev) {
	GetOrCreateFolder(SGamePath + "woowzengine");
	GetOrCreateFolder(SGamePath + "woowzengine/log");
	GetOrCreateFolder(SGamePath + "woowzengine/temporary");

	string SessionInfoPath = SGamePath + "woowzengine/temporary/sessioninfo";
	GetOrCreateFile(SessionInfoPath);
	if (!JSONValid(SessionInfoPath)) { SessionInfoBroken = true; }

	string OurGamePath = SGamePath + "game";
	GetOrCreateFolder(OurGamePath);
	GetOrCreateFolder(OurGamePath+"/engine");
	string JEngine = OurGamePath + "/engine.json";
	GetOrCreateFile(JEngine);
	if (!JSONValid(JEngine)) {
		bool JEngineBroken = true;
		WriteToFile(JEngine, "{}");
	}
	CreateValueJson(JEngine, "Console", "true");
	CreateValueJson(JEngine, "SafeMode", "true");
	CreateValueJson(JEngine, "LogType", "log");
	CreateValueJson(JEngine, "LogFormat", "%y-%mn-%d-%h-%m-%s");
	CreateValueJson(JEngine, "LogStyle", "%b[%s:%m:%h][%t] %c");
	CreateValueJson(JEngine, "LogFatal", "-FATAL");

	string JGame = OurGamePath + "/game.json";
	GetOrCreateFile(JGame);
	if (!JSONValid(JGame)) {
		bool JGameBroken = true;
		WriteToFile(JGame, "{}");
	}
	CreateValueJson(JGame, "Name",    "Example Game");
	CreateValueJson(JGame, "Version", "0.0.0");
	CreateValueJson(JGame, "Author",  "Unknown");

	if (!HasDirectory(OurGamePath + "/start.lua")) {
		HasStartScript = false;
	}
	GetOrCreateFile(OurGamePath + "/start.lua");
	if (!HasStartScript) {
		WriteToFile(OurGamePath + "/start.lua","--[[Example script start.lua\nRuns when the game starts.]]\n\nCheckLua()");
	}

	int Seed = (int)std::time(nullptr);

	map<string, string> SessionInfoInfo = { {"Seed",to_string(Seed)},{"GamePath",SGamePath},{"Version",ev},{"SourcePath",SGamePath + "game/"},{"EngineJson",JEngine},{"GameJson",JGame},{"SessionPath",SGamePath + "woowzengine/temporary/sessioninfo"}};
	WriteToFile(SessionInfoPath,ConvertToJSON(SessionInfoInfo));

	SetRandomSeed(Seed);
}

void Cycle() {
	Render();
}

void GameInstall() {
	LuaCompile();
}

void Install(string ev) {
	BaseInstall(SGamePath);
	CheckFiles(ev);
	LoggerInstall();

	P("ENGINE", "WoowzEngine ["+GetSessionInfo("Version") + "] started!");
	P("ENGINE", "Start game ["+GetGameInfo("Name") + " ("+GetGameInfo("Version") + ")] by [" + GetGameInfo("Author") + "] ");
	if (SafeMode()) {
		P("SAVEMOD", "Safe mode enabled!",10);
	}
	else {
		P("SAVEMOD", "Safe mode disabled!", 14);
	}
	if (SessionInfoBroken) {
		PW("Sessioninfo has corrupted! File has been recreated!", "W0001");
	}
	if (JGameBroken) {
		PW("game.json has corrupted or was not created! File has been recreated!", "W0002");
	}
	if (JEngineBroken) {
		PW("engine.json has corrupted or was not created! File has been recreated!", "W0003");
	}
	if (StringToBool(GetEngineInfo("Console"))) {
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	}

	GLFWInstall();
	GameInstall();
}

void OpenGame(string GamePath_,string EngineVersion_) {
	SGamePath = StringToPath(GamePath_);
	Install(EngineVersion_);

	/*===========[Тестовая зона]===========*/



	/*=====================================*/

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		Cycle();
	}
}