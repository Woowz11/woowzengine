#include <iostream>
#include <string>
#include "Logger.h"
#include "Base.h"
#include "WindowsElements.h"
#include "Files.h"

using namespace std;

string NowLog = "";
string LogsPath = "woowzengine/log/";
string LogsType = "log";

bool LogsSystem = true;
bool LogsFatal_ = false;

string ms, w, y, m, s, h, mn, d;
string LogFormat;

string GetLogName(string replaceFtoThat) {
	return ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(ReplaceString(LogFormat, "%ms", ms), "%w", w), "%y", y), "%mn", mn), "%d", d), "%h", h), "%m", m), "%s", s), "%f", replaceFtoThat);
}

/*Установка логов*/
void LoggerInstall() {
	LogsSystem = StringToBool(GetEngineInfo("Logs"));
	string LogsType_ = GetEngineInfo("LogType");
	if (NameWindowsAccept(LogsType_, true)) {
		LogsType = LogsType_;
	}
	else {
		MessageBoxFatal("LogType (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0011", true);
	}

	if (!NameWindowsAccept(GetEngineInfo("LogFatal"))) {
		MessageBoxFatal("LogFatal (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0015", true);
	}

	LogFormat = GetEngineInfo("LogFormat");
	if(StringEmpty(LogFormat)){ MessageBoxFatal("LogsFormat (engine.json) can't be empty!", "C0012", true); }
	if(!NameWindowsAccept(LogFormat)){ MessageBoxFatal("LogFormat (engine.json) contains illegal characters in windows (\\ / : * ? \" < > | + .)", "C0013", true); }
	ms = GetTime("ms");
	s = GetTime("s");
	m = GetTime("m");
	h = GetTime("h");
	d = GetTime("d");
	mn = GetTime("mn");
	w = GetTime("w");
	y = GetTime("y");
	string LogName = GetLogName("");
	CreateLog(LogName);
}

/*Создать лог*/
void CreateLog(string LogName) {
	if (LogsSystem) {
		NowLog = LogName;
		string log = GetSessionInfo("GamePath") + LogsPath + LogName + "." + LogsType;
		if (HasDirectory(log)) { PF("Log file with that name [" + log + "] already exists! Check LogFormat (engine.json), it could be because of that!", "C0003", true); }
		else {
			GetOrCreateFile(log);
		}
		SetSessionInfo("Log", log);
	}
	else {
		SetSessionInfo("Log", "Logs disabled!");
	}
}

/*Записать в лог*/
void PrintToLog(string Text) {
	if (LogsSystem && !LogsFatal_) {
		if (GetSessionInfo("GamePath") == "ERROR_SESSIONINFO_NOT_FOUND") { MessageBoxFatal("Can't save log, sessioninfo file not found!\n(" + Text + ")", "C0004", true); }
		if (!HasDirectory(GetSessionInfo("GamePath") + LogsPath)) { PF("Folder logs not found!\n(" + Text + ")", "C0005", true); }
		if (!HasDirectory(GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType)) { PF("Log file not found!\n(" + Text + ")", "C0006", true); }
		string Log = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
		WriteToFile(Log, Text, true);
	}
}

/*Обозначает что в логах фатальная ошибка*/
void LogsFatal() {
	if (LogsSystem) {
		LogsFatal_ = true;
		P("LOGGER","Log set type FATAL!");
		string oldlogname = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
		NowLog = GetLogName(GetEngineInfo("LogFatal"));
		string newlogname = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
		SetSessionInfo("Log", newlogname);
		RenameFile(oldlogname, newlogname);
	}
}

/*Обозначает что в логах много ошибок*/
bool LogsErrorsRenamed = false;
void LogsErrors() {
	if (LogsSystem && !LogsFatal_ && !LogsErrorsRenamed) {
		LogsErrorsRenamed = true;
		P("LOGGER", "Log set type ERRORS!");
		string oldlogname = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
		NowLog = GetLogName(GetEngineInfo("LogErrors"));
		string newlogname = GetSessionInfo("GamePath") + LogsPath + NowLog + "." + LogsType;
		SetSessionInfo("Log", newlogname);
		RenameFile(oldlogname, newlogname);
	}
}