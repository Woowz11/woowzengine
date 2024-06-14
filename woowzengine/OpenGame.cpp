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
#include "ResourceManager.h"
#include "Discord.h"
#include "WConst.h"
#include "SessionInfo.h"

using namespace std;

string SGamePath = "";
bool JGameBroken = false;
bool JEngineBroken = false;
bool JSettingsBroken = false;
bool HasStartScript = true;

bool DebugMode = false;/*ЭТО НЕ ТРОГАЙ! DebugMode (DebugVersion) включаеться в Source.cpp!!!*/

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
void DebugPrint_w(wstring text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	wcout << text << "\n";
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

	string TemporaryPath = SGamePath + "woowzengine/temporary";
	string OurGamePath = SGamePath + "game";
	string JEngine = OurGamePath + "/engine.json";
	string JGame = OurGamePath + "/game.json";
	string JSettings = OurGamePath + "/settings.json";

	int Seed = (int)std::time(nullptr);
	
	map<string, string> SessionInfoInfo = { {"Debug",(DebugMode ? "true" : "false")},{"Seed",to_string(Seed)},{"GamePath",SGamePath},{"Version",ev},{"SourcePath",SGamePath + "game/"},{"EngineJson",JEngine},{"GameJson",JGame},{"SettingsJson",JSettings},{"TemporaryPath",TemporaryPath}};
	for (const auto& pair : SessionInfoInfo) {
		string id = pair.first;
		string value = pair.second;
		SetSessionInfoData(id, value);
	}

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

	if (!HasDirectory(ShadersPath + "/font.v")) {
		CreateFile(ShadersPath + "/font.v");
		WriteToFile(ShadersPath + "/font.v", ShaderVertex);
	}
	if (!HasDirectory(ShadersPath + "/font.f")) {
		CreateFile(ShadersPath + "/font.f");
		WriteToFile(ShadersPath + "/font.f", ShaderFragment);
	}

	string FontsPath = EnginePath + "/fonts";
	GetOrCreateFolder(FontsPath);
	if (!HasDirectory(FontsPath + "/default.png")) {
		CreateFile(FontsPath + "/default.png");
		WriteImage(FontsPath + "/default.png", 128, 128, DefaultFontImage, { {'0',l_Color(0,0,0,0)} ,{'1',l_Color(255,255,255)} });
	}

	GetOrCreateFile(JEngine);
	if (!JSONValid(JEngine)) {
		WriteToFile(JEngine, "{}");
	}
	CreateValueJson(JEngine, "Logs", "true");
	CreateValueJson(JEngine, "LogType", "log");
	CreateValueJson(JEngine, "LogFormat", "%y-%mn-%d-%h-%m-%s-%ms%f");
	CreateValueJson(JEngine, "LogStyle", "%b[%h:%m:%s:%ms][%t] %c");
	CreateValueJson(JEngine, "LogFatal", "-FATAL");
	CreateValueJson(JEngine, "LogErrors", "-ERRORS");
	CreateValueJson(JEngine, "OnlyOne", "true");
	CreateValueJson(JEngine, "DiscordActivities", "true");
	CreateValueJson(JEngine, "DiscordApplicationID", "1240635259221970954");
	CreateValueJson(JEngine, "StartScript", "start");

	GetOrCreateFile(JGame);
	if (!JSONValid(JGame)) {
		WriteToFile(JGame, "{}");
	}
	CreateValueJson(JGame, "Name", "Example Game");
	CreateValueJson(JGame, "Version", "0.0.0");
	CreateValueJson(JGame, "Author", "Unknown");

	GetOrCreateFile(JSettings);
	if (!JSONValid(JSettings)) {
		WriteToFile(JSettings, "{}");
	}
	CreateValueJson(JSettings, "Console", "true");
	CreateValueJson(JSettings, "SafeMode", "true");

	SetRandomSeed(Seed);
}

void Cycle() {
	DiscordUpdate();
	Render();
}

void GameInstall() {
	DiscordStart(GetEngineInfo("DiscordApplicationID"));

	if (GetEngineInfo("StartScript") != "" && NameWindowsAccept(GetEngineInfo("StartScript"))) {


		string StartScriptPath = GetSessionInfo("SourcePath") + GetEngineInfo("StartScript") + ".lua";
		if (!HasDirectory(StartScriptPath)) {
			HasStartScript = false;
		}
		GetOrCreateFile(StartScriptPath);
		if (!HasStartScript) {
			WriteToFile(StartScriptPath, StartScriptCode);
		}

		LuaCompile();
	}
	else {
		PF("StartScript ["+ GetEngineInfo("StartScript") +"] cannot be empty or contain incompatible characters with Windows","C0027");
	}
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
	if (JGameBroken) {
		PW("game.json has corrupted or was not created! File has been recreated!", "W0002");
	}
	if (JEngineBroken) {
		PW("engine.json has corrupted or was not created! File has been recreated!", "W0003");
	}
	if (StringToBool(GetSettingsInfo("Console"))) {
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