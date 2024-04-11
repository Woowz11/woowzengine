#include <iostream>
#include <string>
#include <map>
#include "OpenGame.h"
#include "Base.h"
#include "Files.h"
#include "Logger.h"
#include "Easyer.h"

using namespace std;

string SGamePath = "";
bool SessionInfoBroken = false;
bool JGameBroken = false;
bool JEngineBroken = false;

void CheckFiles(string ev) {
	GetOrCreateFolder(SGamePath + "woowzengine");
	GetOrCreateFolder(SGamePath + "woowzengine/log");
	GetOrCreateFolder(SGamePath + "woowzengine/temporary");

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

	if (!JSONValid(SGamePath + "woowzengine/temporary/sessioninfo")) { SessionInfoBroken = true; }

	map<string, string> SessionInfoInfo = { {"GamePath",SGamePath},{"Version",ev},{"SourcePath",SGamePath + "game/"},{"EngineJson",JEngine},{"GameJson",JGame}};
	WriteToFile(SGamePath + "woowzengine/temporary/sessioninfo",ConvertToJSON(SessionInfoInfo));
}

void GameInstall() {

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

	GameInstall();
}

void OpenGame(string GamePath_,string EngineVersion_) {
	SGamePath = StringToPath(GamePath_);
	Install(EngineVersion_);

	P("Hi!!!");
}