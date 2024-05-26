#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include "WindowsElements.h"
#include "OpenGame.h"
#include "Base.h"
#include "Files.h"
#include "Logger.h"
#include "Easyer.h"
#include "LuaCompile.h"
#include "GLFW.h"
#include "Cycles.h"
#include "Discord.h"
#include "WConst.h"

using namespace std;

string SGamePath = "";
bool SessionInfoBroken = false;
bool JGameBroken = false;
bool JEngineBroken = false;
bool HasStartScript = true;
bool DebugMode = false;/*��� �� ������! DebugMode (DebugVersion) ����������� � Source.cpp!!!*/

sol::function GameClosed = sol::nil;

void DebugPrint(string text) {
	if (DebugMode) {
		DebugPrint_(text);
	}
}
void DebugPrint(float f) {
	DebugPrint(to_string(f));
}
void DebugPrint_(string text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	wcout << StringToWString(text) << "\n";
}
void DebugPrint_(float f) {
	DebugPrint_(to_string(f));
}

void SetGameClosedEvent(sol::function f) {
	GameClosed = f;
}

bool WINAPI StopEngine(DWORD CEvent) {
	StartFunction(GameClosed, {});
	DiscordEnd();
	StopCycleEngine();
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

	map<string, string> SessionInfoInfo = { {"Debug",(DebugMode ? "true" : "false")},{"Seed",to_string(Seed)},{"GamePath",SGamePath},{"Version",ev},{"SourcePath",SGamePath + "game/"},{"EngineJson",JEngine},{"GameJson",JGame},{"SessionPath",SessionInfoPath}};
	WriteToFile(SessionInfoPath, ConvertToJSON(SessionInfoInfo));

	GetOrCreateFolder(OurGamePath);

	string EnginePath = OurGamePath + "/engine";
	GetOrCreateFolder(EnginePath);
	if (!HasDirectory(EnginePath + "/default.png")) {
		CreateFile(EnginePath + "/default.png");
		WriteImage(EnginePath + "/default.png", 32, 32, DefaultImage);
	}
	if (!HasDirectory(EnginePath + "/error.png")) {
		CreateFile(EnginePath + "/error.png");
		WriteImage(EnginePath + "/error.png", 32, 32, ErrorImage);
	}

	string ShadersPath = EnginePath + "/shaders";
	GetOrCreateFolder(ShadersPath);
	if (!HasDirectory(ShadersPath + "/default.v")) {
		CreateFile(ShadersPath + "/default.v");
		WriteToFile(ShadersPath + "/default.v", ShaderVertex);
	}
	if (!HasDirectory(ShadersPath + "/default.f")) {
		CreateFile(ShadersPath + "/default.f");
		WriteToFile(ShadersPath + "/default.f", ShaderFragment);
	}

	GetOrCreateFile(JEngine);
	if (!JSONValid(JEngine)) {
		WriteToFile(JEngine, "{}");
	}
	CreateValueJson(JEngine, "Console", "true");
	CreateValueJson(JEngine, "SafeMode", "true");
	CreateValueJson(JEngine, "Logs", "true");
	CreateValueJson(JEngine, "LogType", "log");
	CreateValueJson(JEngine, "LogFormat", "%y-%mn-%d-%h-%m-%s-%ms%f");
	CreateValueJson(JEngine, "LogStyle", "%b[%h:%m:%s:%ms][%t] %c");
	CreateValueJson(JEngine, "LogFatal", "-FATAL");
	CreateValueJson(JEngine, "LogErrors", "-ERRORS");
	CreateValueJson(JEngine, "OnlyOne", "true");
	CreateValueJson(JEngine, "DiscordActivities", "true");
	CreateValueJson(JEngine, "DiscordApplicationID", "1240635259221970954");

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
	DiscordUpdate();
	Render();
}

void GameInstall() {
	DiscordStart(GetEngineInfo("DiscordApplicationID"));
	LuaCompile();
}

void Install(string ev) {
	BaseInstall(SGamePath);

	if (HasDirectory(SGamePath + "game")) {
		for (auto& p : std::filesystem::recursive_directory_iterator(SGamePath + "game")) {
			if (!OnlyASCII(p.path().filename().string())) {
				MessageBoxFatal("Files and folders accept only ASCII characters! [" + p.path().string() + "]", "C0024", true);
			}
		}
	}
	CheckFiles(ev);
	LoggerInstall();

	P("ENGINE", "WoowzEngine [" + GetSessionInfo("Version") + "] started!");
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

	/*===========[�������� ����]===========*/



	/*=====================================*/

	if (StringToBool(GetEngineInfo("OnlyOne"))) {
		CreateMutexA(0, FALSE, StringToLPCSTR(GetSessionInfo("GamePath")+"woowzengine.exe"));
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			P("ENGINE", "Game alredy started!");
			Exit();
		}
	}
	GameInstall();
	P("ENGINE", "Cycles started!");
	CyclePerSecond();
	StartCycleEngine();
}