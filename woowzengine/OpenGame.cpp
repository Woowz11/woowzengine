#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include "OpenGame.h"
#include "Base.h"
#include "Files.h"
#include "Logger.h"
#include "Easyer.h"
#include "LuaCompile.h"
#include "GLFW.h"
#include "Cycles.h"

using namespace std;

string SGamePath = "";
bool SessionInfoBroken = false;
bool JGameBroken = false;
bool JEngineBroken = false;
bool HasStartScript = true;
bool DebugMode = false;/*ЭТО НЕ ТРОГАЙ! DebugMode (DebugVersion) включаеться в Source.cpp!!!*/

sol::function GameClosed = sol::nil;

void DebugPrint(string text) {
	if (DebugMode) {
		cout << text << "\n";
	}
}
void DebugPrint(float f) {
	DebugPrint(to_string(f));
}

void SetGameClosedEvent(sol::function f) {
	GameClosed = f;
}

bool WINAPI StopEngine(DWORD CEvent) {
	StartFunction(GameClosed);
	if (CEvent == CTRL_CLOSE_EVENT) {
		StopGLFW();
		P("ENGINE", "WoowzEngine stopping...");
	}
	return true;
}

void CheckFiles(string ev) {
	GetOrCreateFolder(SGamePath + "woowzengine");
	GetOrCreateFolder(SGamePath + "woowzengine/log");
	GetOrCreateFolder(SGamePath + "woowzengine/temporary");

	string SessionInfoPath = SGamePath + "woowzengine/temporary/sessioninfo";
	string OurGamePath = SGamePath + "game";
	string JEngine = OurGamePath + "/engine.json";
	string JGame = OurGamePath + "/game.json";

	int Seed = (int)std::time(nullptr);

	GetOrCreateFile(SessionInfoPath);

	if (!JSONValid(SessionInfoPath)) {
		SessionInfoBroken = true;
	}

	map<string, string> SessionInfoInfo = { {"Debug",(DebugMode ? "true" : "false")},{"Seed",to_string(Seed)},{"GamePath",SGamePath},{"Version",ev},{"SourcePath",SGamePath + "game/"},{"EngineJson",JEngine},{"GameJson",JGame},{"SessionPath",SGamePath + "woowzengine/temporary/sessioninfo"}};
	WriteToFile(SessionInfoPath, ConvertToJSON(SessionInfoInfo));

	GetOrCreateFolder(OurGamePath);
	GetOrCreateFolder(OurGamePath+"/engine");
	GetOrCreateFile(JEngine);
	if (!JSONValid(JEngine)) {
		WriteToFile(JEngine, "{}");
	}
	CreateValueJson(JEngine, "Console", "true");
	CreateValueJson(JEngine, "SafeMode", "true");
	CreateValueJson(JEngine, "LogType", "log");
	CreateValueJson(JEngine, "LogFormat", "%y-%mn-%d-%h-%m-%s-%ms");
	CreateValueJson(JEngine, "LogStyle", "%b[%h:%m:%s:%ms][%t] %c");
	CreateValueJson(JEngine, "LogFatal", "-FATAL");

	GetOrCreateFile(JGame);
	if (!JSONValid(JGame)) {
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
}

void OpenGame(string GamePath_,string EngineVersion_, bool DebugVersion) {
	SGamePath = StringToPath(GamePath_);
	DebugMode = DebugVersion;
	Install(EngineVersion_);

	/*===========[Тестовая зона]===========*/



	/*=====================================*/

	GameInstall();
	P("ENGINE", "Cycles started!");
	SetCycleEngine();
}