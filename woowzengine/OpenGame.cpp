#include <iostream>
#include <string>
#include <map>
#include "Windows.h"
#include "OpenGame.h"
#include "Base.h"
#include "Files.h"
#include "Logger.h"
#include "Easyer.h"
#include "LuaCompile.h"

using namespace std;

string SGamePath = "";
bool SessionInfoBroken = false;
bool JGameBroken = false;
bool JEngineBroken = false;

void CheckFiles(string ev) {
	GetOrCreateFolder(SGamePath + "woowzengine");
	GetOrCreateFolder(SGamePath + "woowzengine/log");
	GetOrCreateFolder(SGamePath + "woowzengine/temporary");

	string SessionInfoPath = SGamePath + "woowzengine/temporary/sessioninfo";
	GetOrCreateFile(SessionInfoPath);
	if (!JSONValid(SessionInfoPath)) { SessionInfoBroken = true; }

	string OurGamePath = SGamePath + "game";
	GetOrCreateFolder(OurGamePath);
	string JEngine = OurGamePath + "/engine.json";
	GetOrCreateFile(JEngine);
	if (!JSONValid(JEngine)) {
		bool JEngineBroken = true;
		WriteToFile(JEngine, "{}");
	}
	CreateValueJson(JEngine, "Console", "true");
	CreateValueJson(JEngine, "LogType", "log");
	CreateValueJson(JEngine, "LogFormat", "%y-%mn-%d-%h-%m-%s");

	string JGame = OurGamePath + "/game.json";
	GetOrCreateFile(JGame);
	if (!JSONValid(JGame)) {
		bool JGameBroken = true;
		WriteToFile(JGame, "{}");
	}
	CreateValueJson(JGame, "Name",    "Example Game");
	CreateValueJson(JGame, "Version", "0.0.0");
	CreateValueJson(JGame, "Author",  "Unknown");

	map<string, string> SessionInfoInfo = { {"GamePath",SGamePath},{"Version",ev},{"SourcePath",SGamePath + "game/"},{"EngineJson",JEngine},{"GameJson",JGame},{"SessionPath",SGamePath + "woowzengine/temporary/sessioninfo"} };
	WriteToFile(SessionInfoPath,ConvertToJSON(SessionInfoInfo));
}

void GameInstall() {
	LuaCompile();
}

void Install(string ev) {
	BaseInstall(SGamePath);
	CheckFiles(ev);
	LoggerInstall();

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
	GameInstall();
}

void OpenGame(string GamePath_,string EngineVersion_) {
	SGamePath = StringToPath(GamePath_);
	Install(EngineVersion_);

	PP("Hi!!!");
}